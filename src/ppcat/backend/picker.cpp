#include "picker.hpp"
#include "logging.hpp"

#include <fmt/format.h>
#include <unicode/errorcode.h>
#include <unicode/normalizer2.h>
#include <unicode/translit.h>
#include <unicode/unistr.h>
#include <nlohmann/json.hpp>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/icu/utf8.hpp>

#include <filesystem>
#include <optional>
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
    : pegtl::seq<pegtl::not_at<lineend>, paragraph_content, lineend> {};
struct section
    : pegtl::seq<pegtl::plus<paragraph>,
                 pegtl::star<pegtl::seq<pegtl::not_at<pegtl::eof>, lineend>>> {
};
struct whole : pegtl::must<pegtl::star<pegtl::utf8::icu::white_space>,
                           pegtl::plus<section>> {};

using section_type = std::deque<std::string>;
using data_type = std::deque<std::deque<std::string>>;

struct state {
    data_type data;
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

enum struct entity {
    noname,
    article,
    author,
    date,
    chapter,
    preamble,
    epigraph,
    partition,
    summary
};

entity parse_entity(const std::string_view &input) {
    using enum entity;
    entity result = noname;
    auto slug = make_slug(input);

    if (false) {
    } else if (slug.starts_with("статья")) {
        result = article;
    } else if (slug.starts_with("автор") or slug.starts_with("авторы")) {
        result = author;
    } else if (slug.starts_with("дата") or slug.starts_with("день")) {
        result = date;
    } else if (slug.starts_with("глава")) {
        result = chapter;
    } else if (slug.starts_with("преамбула")) {
        result = preamble;
    } else if (slug.starts_with("эпиграф")) {
        result = epigraph;
    } else if (slug.starts_with("раздел")) {
        result = partition;
    } else if (slug.starts_with("краткое_содержание") or
               slug.starts_with("краткое_описание") or
               slug.starts_with("резюме") or slug.starts_with("выжимка") or
               slug.starts_with("описание")) {
        result = summary;
    }

    return result;
}

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::pair<std::string, std::string> break_angles(const std::string &input) {
    std::pair<std::string, std::string> result{};
    std::size_t found = input.find("<");
    result.first =
        input.substr(0, found == std::string::npos ? input.size() : found);
    result.second =
        input.substr(found == std::string::npos ? input.size() : found);
    result.second.erase(0, 1);
    found = result.second.find(">");
    result.second = result.second.substr(
        0, found == std::string::npos ? result.second.size() : found);
    trim(result.first);
    trim(result.second);

    return result;
}

json::json_pointer proc_article(json &data, const json::json_pointer &ptr,
                                section_type &sec) {
    data["type"] = "статья";

    data["title"] = "";
    if (not sec.empty()) {
        data["title"] = sec[0];
        sec.pop_front();
    }
    if (not sec.empty()) {
        auto title_slug = break_angles(sec.back());
        data["short_title"] = title_slug.first;
        data["title_slug"] = title_slug.second;
        sec.pop_back();
    }
    data["subtitle"] = json::array();
    while (not sec.empty()) {
        data["subtitle"].push_back(sec[0]);
        sec.pop_front();
    }

    json::json_pointer new_ptr = ptr;
    data["body"] = json::array();
    new_ptr = ""_json_pointer / "body";

    return new_ptr;
}

json::json_pointer proc_paragraph(json &data, const json::json_pointer &ptr,
                                  section_type &sec) {
    std::ostringstream joined;
    std::copy(sec.begin(), sec.end(),
              std::ostream_iterator<std::string>(joined, "\n"));
    data[ptr].push_back(joined.str());
    sec.clear();

    return ptr;
}

json::json_pointer proc_summary(json &data, const json::json_pointer &ptr,
                                section_type &sec) {
    data["summary"] = json::array();
    while (not sec.empty()) {
        data["summary"].push_back(sec[0]);
        sec.pop_front();
    }
    sec.clear();

    return ptr;
}

json::json_pointer proc_author(json &data, const json::json_pointer &ptr,
                               section_type &sec) {
    while (not sec.empty()) {
        auto name_link = break_angles(sec.back());
        data["author"]["name"] = name_link.first;
        data["author"]["link"] = name_link.second;
        sec.pop_front();
    }
    sec.clear();

    return ptr;
}

json::json_pointer proc_date(json &data, const json::json_pointer &ptr,
                             section_type &sec) {
    while (not sec.empty()) {
        auto date_pubdate = break_angles(sec.back());
        data["date"] = date_pubdate.first;
        data["publication_date"] = date_pubdate.second;
        if (date_pubdate.second.empty()) {
            data["publication_date"] = date_pubdate.first;
        }
        sec.pop_front();
    }
    sec.clear();

    return ptr;
}

json smart_transform(data_type &data) {
    json result = json::object();
    json_pointer ptr = ""_json_pointer;

    while (not data.empty()) {
        std::optional<entity> name;
        while (not data[0].empty()) {
            if (not name.has_value()) {
                name = parse_entity(data[0][0]);
                if (name.value() != entity::noname) {
                    data[0].pop_front();
                }
            } else {
                switch (name.value()) {
                    case entity::noname:
                        if (ptr == ""_json_pointer and
                            (result.find("type") == result.end())) {
                            ptr = proc_article(result, ptr, data[0]);
                            result["type"] = "";
                        } else {
                            ptr = proc_paragraph(result, ptr, data[0]);
                        }
                        break;
                    case entity::article:
                        ptr = proc_article(result, ptr, data[0]);
                        break;
                    case entity::summary:
                        ptr = proc_summary(result, ptr, data[0]);
                        break;
                    case entity::author:
                        ptr = proc_author(result, ptr, data[0]);
                        break;
                    case entity::date:
                        ptr = proc_date(result, ptr, data[0]);
                        break;
                    default:
                        log::critical_throw(fmt::format(
                            "entity with integer value {} not implemented",
                            (int)name.value()));
                }
                name.reset();
            }
        }
        data.pop_front();
    }

    return result;
}

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

    return internal::smart_transform(state.data);
}

nlohmann::json picker::pick(std::string_view input) {
    return pick<std::string_view, pegtl::memory_input<>>(input);
}

nlohmann::json picker::pick(std::istream &input) {
    return pick<std::istream &, pegtl::istream_input<>>(input);
}
