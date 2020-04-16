#ifndef PICKER_HPP
#define PICKER_HPP

#include <nlohmann/json.hpp>
#include <CLI/CLI.hpp>

#include <string_view>
#include <filesystem>

namespace ppcat::backend {

struct picker {
    struct config {
    };

    picker(const config &config);

    static void define_cli(CLI::App &app, config &config);
    nlohmann::json pick(std::string_view input);
};

}

#endif // PICKER_HPP
