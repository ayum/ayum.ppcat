#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "picker.hpp"
#include "applicator.hpp"

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include <tuple>
#include <filesystem>

namespace ppcat::app {

struct backend {
    struct config {
        std::string output;
    };

    template<typename C>
    backend(const C &config)
    : output{std::filesystem::path{std::get<backend::config>(config).output}}
    , _picker{std::get<picker::config>(config)}
    , _applicator{std::get<applicator::config>(config)}
    {
        if (output.empty()) {
            std::filesystem::path templet = std::get<applicator::config>(config).templet;
            std::filesystem::path input = std::get<picker::config>(config).input;
            if (templet.empty()) {
                input.replace_extension(".json");
                output = input;
            } else {
                if (templet.has_extension()) {
                    templet.replace_extension("");
                } else {
                    templet.replace_extension(".output");
                }
                output = templet;
            }
        }

        if (not std::get<applicator::config>(config).templet.empty()) {
            apply = true;
        }
    }

    static void define_cli(CLI::App &app, config &config);
    void run();

private:
    std::filesystem::path output;
    picker _picker;
    applicator _applicator;
    bool apply = false;
};

}


#endif // BACKEND_HPP
