#ifndef SCRIPTING_ENGINE_HPP
#define SCRIPTING_ENGINE_HPP

#include "engine.startup.hpp"

#include <CLI/CLI.hpp>
#include <chaiscript/chaiscript.hpp>
#include <mio/mmap.hpp>

#include <optional>
#include <string_view>

namespace ppcat::engine::scripting {

struct engine {
    struct config {
        std::string preload_path;
        std::string preload_content;
        std::optional<std::string> startup_path;
    };

    engine(const config &config);

    static void define_cli(CLI::App &app, config &config);
    void eval(std::string_view input);
    void start();

private:
    std::string_view preload;
    std::string_view startup;
    mio::mmap_source preload_mmap;
    std::string preload_content;
    mio::mmap_source startup_mmap;

    chaiscript::ChaiScript chai;
};

}  // namespace ppcat::engine::scripting

#endif  // SCRIPTING_ENGINE_HPP
