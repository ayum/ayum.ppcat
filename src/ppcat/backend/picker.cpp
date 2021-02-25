#include "picker.hpp"
#include "logging.hpp"

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

std::string make_slug(std::string_view input) {
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

    std::string result;
    uinput.toUTF8String(result);

    return result;
}

using namespace tao;

struct linebreak
    : pegtl::utf8::one<U'\u000a', U'\u000c', U'\u2028', U'\u2029'> {};
struct whitespace
    : pegtl::seq<pegtl::not_at<linebreak>, pegtl::utf8::icu::white_space> {};
struct character : pegtl::seq<pegtl::not_at<linebreak>, pegtl::utf8::any> {};
struct lineend
    : pegtl::seq<pegtl::star<whitespace>, pegtl::sor<linebreak, pegtl::eof>> {};
struct paragraph_content
    : pegtl::plus<pegtl::seq<pegtl::not_at<lineend>, character>> {};
struct paragraph
    : pegtl::seq<pegtl::not_at<lineend>,
                 pegtl::pad<paragraph_content, whitespace>, lineend> {};
struct section
    : pegtl::seq<pegtl::plus<paragraph>,
                 pegtl::star<pegtl::seq<pegtl::not_at<pegtl::eof>, lineend>>> {
};
struct whole : pegtl::must<pegtl::star<pegtl::utf8::icu::white_space>,
                           pegtl::plus<section>> {};

struct state {
    std::vector<std::deque<std::string>> data;
    bool new_section = true;
};

template <typename R>
struct action : tao::pegtl::nothing<R> {};

template <>
struct action<paragraph_content> : pegtl::discard_input_on_success {
    template <typename I>
    static void apply(const I &in, state &s) {
        std::string content{in.iterator().data, in.input().current()};
        if (s.new_section) {
            s.data.push_back({});
            s.new_section = false;
        }
        auto &paragraphs = s.data.back();
        paragraphs.push_back(content);
    }
};

template <>
struct action<section> : pegtl::discard_input_on_success {
    template <typename I>
    static void apply(const I &in, state &s) {
        (void)in;
        s.new_section = true;
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
