module;

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <fmt/format.h>

#include <filesystem>
#include <source_location>
#include <string>
#include <string_view>

export module log;

export namespace ppcat::common::log {

inline constexpr std::string_view default_level = "error";

enum struct level { trace, debug, info, warning, error, critical, off };

spdlog::level::level_enum to_spdlog_level(level level) {
    switch (level) {
        case level::trace:
            return spdlog::level::level_enum::trace;
        case level::debug:
            return spdlog::level::level_enum::debug;
        case level::info:
            return spdlog::level::level_enum::info;
        case level::warning:
            return spdlog::level::level_enum::warn;
        case level::error:
            return spdlog::level::level_enum::err;
        case level::critical:
            return spdlog::level::level_enum::critical;
        case level::off:
            return spdlog::level::level_enum::off;
    }

    throw std::runtime_error{
        fmt::format("{}", "Error while converting log level to sdplog level")};
}

level from_str(std::string_view str) {
    if (false) {
    } else if ("trace" == str)
        return level::trace;
    else if ("debug" == str)
        return level::debug;
    else if ("info" == str)
        return level::info;
    else if ("warning" == str)
        return level::warning;
    else if ("error" == str)
        return level::error;
    else if ("critical" == str)
        return level::critical;
    else if ("off" == str)
        return level::off;

    throw std::runtime_error{
        fmt::format("String '{}' cannot be converted to log level", str)};
}

std::string to_str(level level) {
    if (false) {
    } else if (level::trace == level)
        return "trace";
    else if (level::debug == level)
        return "debug";
    else if (level::info == level)
        return "info";
    else if (level::warning == level)
        return "warning";
    else if (level::error == level)
        return "error";
    else if (level::critical == level)
        return "critical";
    else if (level::off == level)
        return "off";

    throw std::runtime_error{
        fmt::format("Error while converting log level to string")};
}

void set_level(level level) { spdlog::set_level(to_spdlog_level(level)); }

void set_level(std::string level = std::string{default_level}) {
    set_level(from_str(level));
}

void set_file(std::filesystem::path path) {
    if (not path.empty()) {
        spdlog::set_default_logger(
            spdlog::basic_logger_mt("basic_logger", path.string()));
    }
}

void initialize(std::filesystem::path path, level level) {
    set_file(path);
    set_level(level);
}

void initialize(std::filesystem::path path = {},
                std::string level = std::string{default_level}) {
    initialize(path, from_str(level));
}

void log(
    level level, std::string_view msg,
    const std::source_location& location = std::source_location::current()) {
    if (level <= level::debug) {
        spdlog::log(to_spdlog_level(level), "{}\nIn {}:{}", msg,
                    location.file_name(), location.line());
    } else {
        spdlog::log(to_spdlog_level(level), "{}", msg);
    }
}

void trace(std::string_view msg, const std::source_location& location =
                                std::source_location::current()) {
    log(level::trace, msg, location);
}

void debug(std::string_view msg, const std::source_location& location =
                                std::source_location::current()) {
    log(level::debug, msg, location);
}

void info(std::string_view msg, const std::source_location& location =
                               std::source_location::current()) {
    log(level::info, msg, location);
}

void warning(std::string_view msg, const std::source_location& location =
                                  std::source_location::current()) {
    log(level::warning, msg, location);
}

void error(std::string_view msg, const std::source_location& location =
                                std::source_location::current()) {
    log(level::error, msg, location);
}

void critical(std::string_view msg, const std::source_location& location =
                                   std::source_location::current()) {
    log(level::critical, msg, location);
}

void critical_throw(std::string_view msg, const std::source_location& location =
                                         std::source_location::current()) {
    log(level::critical, msg, location);
    throw std::runtime_error(std::string(msg));
}

}