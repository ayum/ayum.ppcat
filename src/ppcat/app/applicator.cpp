#include "applicator.hpp"
#include "logging.hpp"

#include <inja/inja.hpp>
#include <fmt/format.h>
#include <CLI/CLI.hpp>

#include <regex>
#include <string_view>
#include <filesystem>

using namespace ppcat::app;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;

applicator::applicator(const applicator::config &config)
    : path(config.templet)
{
}

void applicator::define_cli(CLI::App &app, config &config) {
    app.add_option("-t,--template" , config.templet, "Template to apply to with input files");
}

void applicator::apply(std::ostream& os, const nlohmann::json &data) {
    os << data;
}
