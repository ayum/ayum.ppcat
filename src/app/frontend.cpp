#include "frontend.hpp"
#include "picker.hpp"

#include <fmt/format.h>
#include <CLI/CLI.hpp>
#include <mio/mmap.hpp>

#include <filesystem>
#include <fstream>
#include <string>

import log;

using namespace app;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;

void frontend::define_cli(CLI::App &app, frontend::config &config) {
    app.add_option(
        "-o,--output", config.output,
        "Ouput rendered file path, if empty then template without extension");
    app.add_option("file", config.input,
                   "File to read template parameters from")
        ->required();
}

void frontend::run() {
    std::ofstream file(output, std::ofstream::out | std::ofstream::trunc);
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    if (filesystem::is_empty(input)) {
        return;
    }

    mio::mmap_source mmap(input.string());

    _engine.start();

    nlohmann::json data = _picker.pick({mmap.begin(), mmap.end()});
    file << data.dump(2);
}
