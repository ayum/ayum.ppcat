#include "picker.hpp"
#include "logging.hpp"

#include <mio/mmap.hpp>
#include <fmt/format.h>

#include <regex>
#include <string_view>
#include <filesystem>

using namespace ppcat::cli;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;

picker::picker(const filesystem::path &path)
    : path(path)
{
}

json picker::pick() {
    if (filesystem::is_empty(path)) {
        return {};
    }

    json data;
    json_pointer<json> ptr;

    mio::mmap_source mmap(path.string());

    std::regex chunk_re(R"((^([^[:graph:]]*\n)*|([^[:graph:]]*\n){2,}|[^[:graph:]]*$))");
    for (auto &&it  = std::cregex_token_iterator(mmap.begin(), mmap.end(), chunk_re, -1);
                it != std::cregex_token_iterator(); ++it) {
        if (it->first == it->second) {
            continue;
        }
        std::string_view chunk{it->first, it->second};
        log::trace(fmt::format("Chunk:\n'{}'", chunk));

        auto parse = [](std::string_view chunk, json &data, json_pointer<json> &ptr) -> void {
            auto parse_impl = [](auto &parse_ref, std::string_view chunk, json &data, json_pointer<json> &ptr) -> void {
                std::regex strip_re(R"(^[^[:graph:]]*([\s\S]*?)[^[:graph:]]*$)");
                std::regex pick_re(R"(^[^[:graph:]]*<(.*?)>(:?)[^[:graph:]]*([\s\S]*))");
                std::cmatch match;

                std::regex_match(chunk.begin(), chunk.end(), match, strip_re);
                if (match.empty()) {
                    log::warning(fmt::format("No match when stripping chunk '{}'", chunk));
                } else {
                    chunk = {match[1].first, match[1].second};
                }

                std::regex_match(chunk.begin(), chunk.end(), match, pick_re);
                if (match.empty()) {
                    data[ptr]["default"].push_back(chunk);
                } else {
                    std::string_view key{match[1].first, match[1].second};
                    std::string_view colon{match[2].first, match[2].second};
                    std::string_view value{match[3].first, match[3].second};
                    log::trace(fmt::format("Match:\nkey='{}'\ncolon='{}'\nvalue='{}'", key, colon, value));
                    bool nested = std::regex_match(value.begin(), value.end(), match, pick_re);
                    if (not colon.empty()) {
                        if (nested) {
                            data[ptr][std::string(key)] = "";
                        } else {
                            data[ptr][std::string(key)] = value;
                        }
                    } else {
                        json_pointer p = ptr;
                        for (; not p.empty(); p = p.parent_pointer()) {
                            if (p.back() == key) {
                                ptr = p / data[p].size();
                                data[ptr] = json::object();
                                break;
                            }
                        }
                        if (p.empty()) {
                            ptr /= std::string{key};
                            data[ptr] = json::array();
                            ptr /= 0;
                        }
                    }
                    if (nested) {
                        parse_ref(parse_ref, value, data, ptr);
                    }
                }
                log::trace(fmt::format("Data:\njson='{}'", data.dump()));
            };
            parse_impl(parse_impl, chunk, data, ptr);
        };
        parse(chunk, data, ptr);
    }

    return data;
}
