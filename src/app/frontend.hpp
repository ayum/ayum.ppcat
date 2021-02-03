#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include "picker.hpp"
#include "scripting/engine.hpp"

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <tuple>

namespace app {

struct frontend {
    struct config {
        std::string output;
        std::string input;
    };

    using frontend_types = std::tuple<frontend, ppcat::backend::picker,
                                      ppcat::engine::scripting::engine>;

    template <typename C>
    frontend(const C &config)
        : output{std::filesystem::path{
              std::get<frontend::config>(config).output}},
          input{
              std::filesystem::path{std::get<frontend::config>(config).input}},
          _picker{std::get<ppcat::backend::picker::config>(config)},
          _engine{std::get<ppcat::engine::scripting::engine::config>(config)} {
        if (output.empty()) {
            output = input;
            output.replace_extension(".json");
        };
    }

    static void define_cli(CLI::App &app, config &config);
    void run();

private:
    std::filesystem::path output;
    std::filesystem::path input;
    ppcat::backend::picker _picker;
    ppcat::engine::scripting::engine _engine;
};

}  // namespace app

#endif  // FRONTEND_HPP
