#include "engine.hpp"

#include <CLI/CLI.hpp>
#include <mio/mmap.hpp>

#include <filesystem>
#include <string_view>

import log;

using namespace ppcat::common;
using namespace ppcat::engine::scripting;
namespace filesystem = std::filesystem;

engine::engine(const engine::config &config) {
    if (not config.preload_content.empty()) {
        preload_content = config.preload_content;
        preload = preload_content;
    }

    if (not config.preload_path.empty()) {
        std::error_code error;
        preload_mmap.map(config.preload_path, error);
        if (error) {
            throw error;
        }
        preload = {preload_mmap.begin(), preload_mmap.end()};
    }

    if (config.startup_path) {
        if (not config.startup_path.value().empty()) {
            std::error_code error;
            startup_mmap.map(config.startup_path.value(), error);
            if (error) {
                throw error;
            }
            startup = {startup_mmap.begin(), startup_mmap.end()};
        }
    } else {
        startup = startup_script;
    }
}

void engine::define_cli(CLI::App &app, config &config) {
    app.add_option("-p,--preload-path", config.preload_path,
                   "Path to script to load befor all");
    app.add_option("-e,--preload-content", config.preload_content,
                   "Content of script to load befor all");
    app.add_option("-s,--startup-path", config.startup_path,
                   "Path to script to load instaed startup script");
}

void engine::start() {
    if (not preload.empty()) {
        chai.eval(preload.data());
    }

    chai.eval(startup.data());
}

void engine::eval(std::string_view input) {
    if (not input.empty()) {
        chai.eval(input.data());
    }
}
