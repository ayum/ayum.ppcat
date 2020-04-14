#include "backend.hpp"
#include "picker.hpp"
#include "logging.hpp"

#include <CLI/CLI.hpp>
#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <string>

using namespace ppcat::backend;
using namespace ppcat::common;
using namespace nlohmann;
namespace filesystem = std::filesystem;

void backend::define_cli(CLI::App &app, backend::config &config) {
    app.add_option("-o,--output", config.output, "Ouput rendered file path, if empty then template without extension");
}

void backend::run() {
    std::ofstream file(output, std::ofstream::out | std::ofstream::trunc);
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    json data = _picker.pick();
    file << data.dump(2);
}
