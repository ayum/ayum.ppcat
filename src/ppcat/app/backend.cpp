#include "backend.hpp"
#include "picker.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <fstream>

using namespace ppcat::app;
using namespace nlohmann;
namespace filesystem = std::filesystem;

backend::backend(const backend::config &config)
    : _config(config)
{
}

void backend::run() {
    run_once(filesystem::path{"/tmp/1"});
}

void backend::run_once(filesystem::path output) {
    json data = picker{get<picker::config>(_config)}.pick();

    std::ofstream file(output, std::ofstream::out | std::ofstream::trunc);
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit );
    applicator{get<applicator::config>(_config)}.apply(file, data);
}
