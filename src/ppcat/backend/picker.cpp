#include "picker.hpp"
#include "logging.hpp"

#include <fmt/format.h>

#include <regex>
#include <string_view>
#include <filesystem>

using namespace ppcat::backend;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;

picker::picker(const picker::config &config) {
    (void)config;
}

void picker::define_cli(CLI::App &app, config &config) {
    (void)app;
    (void)config;
}

json picker::pick(std::string_view input) {
    json data;
    json_pointer<json> ptr;

    std::regex chunk_re(R"((^(\s*\n)*|(\s*\n){2,}|\s*$))");
    for (auto &&it  = std::cregex_token_iterator(input.begin(), input.end(), chunk_re, -1);
                it != std::cregex_token_iterator(); ++it) {
        if (it->first == it->second) {
            continue;
        }
        std::string_view chunk{it->first, it->second};
        log::trace(fmt::format("Chunk:\n'{}'", chunk));

        auto parse = [](std::string_view chunk, json &data, json_pointer<json> &ptr) -> void {
            auto parse_impl = [](auto &parse_ref, std::string_view chunk, json &data, json_pointer<json> &ptr) -> void {
                std::regex strip_re(R"(^\s*([\s\S]*?)\s*$)");
                std::regex pick_re(R"(^\s*<(.*?)>(:?)[^\S\n]*(.*)\n?([\s\S]*))");
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
                    std::string_view tail{match[4].first, match[4].second};
                    log::trace(fmt::format("Match:\nkey='{}'\ncolon='{}'\nvalue='{}'\ntail='{}'", key, colon, value, tail));
                    if (colon.empty() && not value.empty()) {
                        log::critical_throw("Non empy same line tag value without colon. Examine under trace.");
                    }
                    bool nested = std::regex_match(tail.begin(), tail.end(), match, pick_re);
                    if (not colon.empty()) {
                        if (not value.empty()) {
                            data[ptr][std::string(key)] = value;
                        } else {
                            if (nested) {
                                data[ptr][std::string(key)] = "";
                            } else {
                                data[ptr][std::string(key)] = tail;
                                tail = {};
                            }
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
                    if (not tail.empty()) {
                        parse_ref(parse_ref, tail, data, ptr);
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
