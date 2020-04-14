#ifndef PICKER_HPP
#define PICKER_HPP

#include <nlohmann/json.hpp>
#include <CLI/CLI.hpp>

#include <vector>
#include <string>
#include <filesystem>

namespace ppcat::backend {

struct picker {
    struct config {
        std::string input;
    };

    picker(const config &config);

    static void define_cli(CLI::App &app, config &config);
    nlohmann::json pick();

private:
    std::filesystem::path input;
};

}

#endif // PICKER_HPP
