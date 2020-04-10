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
            std::filesystem::path path = std::get<applicator::config>(config).templet;
            if (path.has_extension()) {
                path.replace_extension("");
            } else if (path.empty()) {
                path = "output";
            } else {
                path.replace_extension(".output");
            }
            output = path;
        }
    }

    static void define_cli(CLI::App &app, config &config);
    void run();

private:
    std::filesystem::path output;
    picker _picker;
    applicator _applicator;

};

}


#endif // BACKEND_HPP
