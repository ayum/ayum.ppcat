#include "logging.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <fmt/format.h>

#include <filesystem>
#include <string>
#include <experimental/source_location>


using namespace ppcat::common;
using namespace std;

log::level log::from_str(std::string_view str) {
    if (false) {}
    else if ("trace"    == str) return log::level::trace;
    else if ("debug"    == str) return log::level::debug;
    else if ("info"     == str) return log::level::info;
    else if ("warning"  == str) return log::level::warning;
    else if ("error"    == str) return log::level::error;
    else if ("critical" == str) return log::level::critical;
    else if ("off"      == str) return log::level::off;

    throw std::runtime_error{fmt::format("String '{}' cannot be converted to log level", str)};
}

std::string log::to_str(log::level level) {
    if (false) {}
    else if (log::level::trace    == level) return "trace";
    else if (log::level::debug    == level) return "debug";
    else if (log::level::info     == level) return "info";
    else if (log::level::warning  == level) return "warning";
    else if (log::level::error    == level) return "error";
    else if (log::level::critical == level) return "critical";
    else if (log::level::off      == level) return "off";

    throw std::runtime_error{fmt::format("Error while converting log level to string")};
}

static
spdlog::level::level_enum to_spdlog_level(log::level level) {
    switch (level) {
    case log::level::trace: return spdlog::level::level_enum::trace;
    case log::level::debug: return spdlog::level::level_enum::debug;
    case log::level::info: return spdlog::level::level_enum::info;
    case log::level::warning: return spdlog::level::level_enum::warn;
    case log::level::error: return spdlog::level::level_enum::err;
    case log::level::critical: return spdlog::level::level_enum::critical;
    case log::level::off: return spdlog::level::level_enum::off;
    }

    throw std::runtime_error{fmt::format("{}", "Error while converting log level to sdplog level")};
}

void log::initialize(std::filesystem::path path, level level) {
    set_file(path);
    set_level(level);
}

void log::initialize(std::filesystem::path path, std::string level) {
    log::initialize(path, log::from_str(level));
}

void log::log(log::level level, std::string_view msg, const std::experimental::source_location& location) {
    if (level <= log::level::debug) {
        spdlog::log(to_spdlog_level(level), "{}\nIn {}:{}", msg, location.file_name(), location.line());
    } else {
        spdlog::log(to_spdlog_level(level), "{}", msg);
    }
}

void log::set_level(level level) {
    spdlog::set_level(to_spdlog_level(level));
}

void log::set_level(std::string level) {
    set_level(log::from_str(level));
}

void log::set_file(std::filesystem::path path) {
    if (not path.empty()) {
        spdlog::set_default_logger(spdlog::basic_logger_mt("basic_logger", path.string()));
    }
}

void log::trace(string_view msg, const std::experimental::source_location& location) {
    log::log(log::level::trace, msg, location);
}

void log::debug(string_view msg, const std::experimental::source_location& location) {
    log::log(log::level::debug, msg, location);
}

void log::info(string_view msg, const std::experimental::source_location& location) {
    log::log(log::level::info, msg, location);
}

void log::warning(string_view msg, const std::experimental::source_location& location) {
    log::log(log::level::warning, msg, location);
}

void log::error(string_view msg, const std::experimental::source_location& location) {
    log::log(log::level::error, msg, location);
}

void log::critical(string_view msg, const std::experimental::source_location& location) {
    log::log(log::level::critical, msg, location);
}
