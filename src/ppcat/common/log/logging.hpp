
#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <experimental/source_location>

namespace ppcat::common::log {

constexpr std::string_view default_level = "error";

enum struct level {
    trace,
    debug,
    info,
    warning,
    error,
    critical,
    off
};

level from_str(std::string_view str);
std::string to_str(level level);

void initialize(std::filesystem::path path, level level);
void initialize(std::filesystem::path path = {}, std::string level = std::string{default_level});

void set_level(level level);
void set_level(std::string level = std::string{default_level});
void set_file(std::filesystem::path path);

void log(level level, std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());
void trace(std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());
void debug(std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());
void info(std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());
void warning(std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());
void error(std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());
void critical(std::string_view msg, const std::experimental::source_location& location = std::experimental::source_location::current());

}

#endif /* LOGGING_HPP */
