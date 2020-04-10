#include "applicator.hpp"
#include "logging.hpp"

#include <inja/inja.hpp>
#include <fmt/format.h>

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

void applicator::apply(std::ostream& os, const nlohmann::json &data) {
    os << data;
}
