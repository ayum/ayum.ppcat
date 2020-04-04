#include "backend.hpp"

#include <fmt/format.h>

using namespace ppcat::cli;
using namespace nlohmann;
using namespace std;

backend::backend(const backend::config &config)
    : _config(config)
{
}

void backend::run() {
    for (auto &&file: _config.files) {
        run_once(file);
    }
}

json backend::run_once(filesystem::path path) {
    fmt::print("{}\n", path.string());
    return {};
}
