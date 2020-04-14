#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "picker.hpp"

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include <tuple>
#include <filesystem>

namespace ppcat::backend {

struct backend {
    struct config {
        std::string output;
    };

    template<typename C>
    backend(const C &config)
    : output{std::filesystem::path{std::get<backend::config>(config).output}}
    , _picker{std::get<picker::config>(config)}
    {
        if (output.empty()) {
            std::filesystem::path input = std::get<picker::config>(config).input;
            input.replace_extension(".json");
            output = input;
        }
    }

    static void define_cli(CLI::App &app, config &config);
    void run();

private:
    std::filesystem::path output;
    picker _picker;
};

}


#endif // BACKEND_HPP
