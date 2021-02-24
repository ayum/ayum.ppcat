#include "picker.hpp"

#include <fmt/format.h>
#include <unicode/errorcode.h>
#include <unicode/normalizer2.h>
#include <unicode/translit.h>
#include <unicode/unistr.h>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/icu/utf8.hpp>

#include <filesystem>
#include <regex>
#include <string_view>
#include <utility>

import log;

using namespace ppcat::backend;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;

picker::picker(const picker::config &config)
    : buffer_size(config.buffer_size) {}

void picker::define_cli(CLI::App &app, config &config) {
    app.add_option("-b,--buffer-size", config.buffer_size,
                   "Buffer size when parsing large inputs");
}

namespace ppcat::backend::internal {

std::pair<std::string, std::string> make_slug(std::string_view input) {
    icu::UnicodeString uinput{input.data(),
                              static_cast<int32_t>(input.length()), "UTF-8"};

    icu::ErrorCode ec;
    const icu::Normalizer2 *nfc = icu::Normalizer2::getNFCInstance(ec);
    uinput = nfc->normalize(uinput, ec);
    uinput.toLower();

    UParseError pe;
    const icu::Transliterator *trans = icu::Transliterator::createFromRules(
        "",
        "[^[:Lowercase Letter:][:Number:]]+ > '_'; :: Any-Null; ^ { '_'+ > ; "
        "'_'+ } $ > ;",
        UTRANS_FORWARD, pe, ec);
    if (ec.isFailure()) {
        log::critical_throw(
            fmt::format("Cannot create transliterator: {}", ec.errorName()));
    }
    trans->transliterate(uinput);

    icu::UnicodeString ufirst = uinput;
    const icu::Transliterator *trans_first =
        icu::Transliterator::createFromRules(
            "first",
            "'_' { [[:Number:]n]+ } $ > ; :: Any-Null;"
            "'_'+ } $ > ;",
            UTRANS_FORWARD, pe, ec);
    if (ec.isFailure()) {
        log::critical_throw(
            fmt::format("Cannot create transliterator: {}", ec.errorName()));
    }
    trans_first->transliterate(ufirst);

    std::string first;
    ufirst.toUTF8String(first);

    if (ufirst.endsWith(uinput)) {
        return {first, ""};
    }

    icu::UnicodeString usecond = uinput;
    usecond.removeBetween(0, ufirst.length());
    trans->transliterate(usecond);

    std::string second;
    usecond.toUTF8String(second);

    return {first, second};
}

using namespace tao;

struct leftangle : pegtl::one<'<'> {};
struct rightangle : pegtl::one<'>'> {};
struct linebreak
    : pegtl::utf8::one<U'\u000a', U'\u000c', U'\u2028', U'\u2029'> {};
struct whitespace
    : pegtl::seq<pegtl::not_at<linebreak>, pegtl::utf8::icu::white_space> {};
struct character : pegtl::seq<pegtl::not_at<linebreak>, pegtl::utf8::any> {};
struct lineend
    : pegtl::seq<pegtl::star<whitespace>, pegtl::sor<linebreak, pegtl::eof>> {};
struct key : pegtl::seq<pegtl::not_at<rightangle>,
                        pegtl::until<pegtl::at<rightangle>, character>> {};
struct tag : pegtl::seq<leftangle, key, rightangle> {};
struct emptytag : pegtl::seq<leftangle, rightangle> {};
struct padtag : pegtl::pad<tag, whitespace> {};
struct valueend
    : pegtl::seq<lineend, pegtl::sor<pegtl::eof, pegtl::rep_min<1, lineend>,
                                     pegtl::at<padtag>>> {};
struct section : pegtl::seq<padtag, valueend> {};
struct value : pegtl::seq<pegtl::not_at<valueend>,
                          pegtl::until<pegtl::at<valueend>, pegtl::utf8::any>> {
};
struct attribute : pegtl::seq<padtag, value, valueend> {};
struct paragraph : pegtl::seq<value, valueend> {};
struct whole
    : pegtl::must<pegtl::star<pegtl::utf8::icu::white_space>, section,
                  pegtl::star<pegtl::sor<section, attribute, paragraph>>> {};

struct state {
    std::string key;
    std::string value;
    nlohmann::json data;
    nlohmann::json_pointer<nlohmann::json> ptr;
};

template <typename R>
struct action : tao::pegtl::nothing<R> {};

template <>
struct action<key> {
    template <typename I>
    static void apply(const I &in, state &s) {
        std::string pick{in.iterator().data, in.input().current()};
        s.key = pick;
    }
};

template <>
struct action<value> {
    template <typename I>
    static void apply(const I &in, state &s) {
        std::string pick{in.iterator().data, in.input().current()};
        s.value = pick;
    }
};

template <>
struct action<section> : pegtl::discard_input_on_success {
    template <typename I>
    static void apply(const I &in, state &s) {
        (void)in;

        auto key = make_slug(s.key);
        nlohmann::json_pointer p = s.ptr;
        std::string last;
        for (; not p.empty(); p = p.parent_pointer()) {
            if (p.back() == key.first) {
                break;
            }
            last = p.back();
        }
        if (p.empty()) {
            s.ptr /= key.first;
            if (not key.second.empty()) {
                s.data[s.ptr] = nlohmann::json::array();
                s.ptr /= 0;
            } else {
                s.data[s.ptr] = nlohmann::json::object();
            }
        } else {
            bool isArray =
                not last.empty() and
                last.find_first_not_of("0123456789") == std::string::npos;
            if (isArray) {
                s.ptr = p / last;
            } else {
                s.ptr = p;
            }
            if (not key.second.empty()) {
                if (not isArray) {
                    log::critical_throw(
                        fmt::format("Data is not array for pointer \'{}\'",
                                    s.ptr.to_string()));
                }
                nlohmann::json_pointer parent = s.ptr.parent_pointer();
                s.ptr = parent / s.data[parent].size();
            }
        }
        log::trace(
            fmt::format("Section:\nkey first={}\nkey second={}\npointer={}\n",
                        key.first, key.second, s.ptr.to_string()));
    }
};

template <>
struct action<attribute> : pegtl::discard_input_on_success {
    template <typename I>
    static void apply(const I &in, state &s) {
        (void)in;

        auto key = make_slug(s.key);
        nlohmann::json_pointer p = s.ptr / key.first;
        if (key.second.empty()) {
            s.data[p] = s.value;
        } else {
            s.data[p].push_back(s.value);
        }
        log::trace(
            fmt::format("Attribute:\nkey first={}\nkey second={}\nvalue={}\n",
                        key.first, key.second, s.data[p].dump()));
    }
};

template <>
struct action<paragraph> : pegtl::discard_input_on_success {
    template <typename I>
    static void apply(const I &in, state &s) {
        (void)in;

        nlohmann::json_pointer p = s.ptr / "paragraph";
        s.data[p].push_back(s.value);
        log::trace(fmt::format("Paragraph:\n{}\n", s.data[p].dump()));
    }
};

}  // namespace ppcat::backend::internal

using namespace tao;

template <>
inline pegtl::memory_input<>
picker::parse_input<std::string_view, pegtl::memory_input<>>(
    std::string_view input) {
    return {input.begin(), input.end(), ""};
}

template <>
inline pegtl::istream_input<>
picker::parse_input<std::istream &, pegtl::istream_input<>>(
    std::istream &input) {
    return {input, buffer_size, ""};
}

template <typename I, typename T>
inline nlohmann::json picker::pick(I input) {
    internal::state state;
    T _input = parse_input<I, T>(input);
    pegtl::parse<internal::whole, internal::action>(_input, state);

    return state.data;
}

nlohmann::json picker::pick(std::string_view input) {
    return pick<std::string_view, pegtl::memory_input<>>(input);
}

nlohmann::json picker::pick(std::istream &input) {
    return pick<std::istream &, pegtl::istream_input<>>(input);
}
