#include "applicator.hpp"
#include "logging.hpp"

#include <kainjow/mustache/mustache.hpp>
#include <fmt/format.h>
#include <CLI/CLI.hpp>

#include <regex>
#include <string_view>
#include <filesystem>
#include <fstream>

using namespace ppcat::app;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;
namespace mustache = kainjow::mustache;

applicator::applicator(const applicator::config &config)
    : templet(config.templet)
{
}

mustache::data convert(const nlohmann::json &data) {
    auto convert_impl = [](auto &convert_ref, const nlohmann::json &data) -> mustache::data {
        switch (data.type()) {
        case json::value_t::string:
        {
            return {data.get<std::string>()};
        }
        break;
        case json::value_t::array:
        {
            mustache::data result{mustache::data::type::list};
            for (auto it = data.begin(); it != data.end(); ++it) {
                result.push_back(convert_ref(convert_ref, it.value()));
            }
            return result;
        }
        break;
        case json::value_t::object:
        {
            mustache::data result{mustache::data::type::object};
            for (auto it = data.begin(); it != data.end(); ++it) {
                result[it.key()] = convert_ref(convert_ref, it.value());
            }
            return result;
        }
        break;
        default:
            log::critical_throw("Picked json data is not valid. Use trace log level to inspect it");
        }

        log::critical_throw("Have reached unreachable");
    };
    return convert_impl(convert_impl, data);
}

void applicator::define_cli(CLI::App &app, config &config) {
    app.add_option("-t,--template" , config.templet, "Template to apply to with input files")->required();
}

void applicator::apply(std::ostream& os, const nlohmann::json &data) {
    std::ifstream ifs(templet.string());
    std::string content{std::istreambuf_iterator<char>(ifs),
                        std::istreambuf_iterator<char>()};

    mustache::mustache tmpl(content);
    tmpl.render(convert(data), os);
}
