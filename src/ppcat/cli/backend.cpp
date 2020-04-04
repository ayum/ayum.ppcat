#include "backend.hpp"
#include "picker.hpp"

#include <fmt/format.h>

using namespace ppcat::cli;
using namespace nlohmann;
using namespace std;

backend::backend(const backend::config &config)
    : _config(config)
{
}

void backend::run() {
    for (auto &&file: get<picker::config>(_config).files) {
        run_once(file);
    }
}

void backend::run_once(filesystem::path path) {
    json data = picker(path).pick();
    (void)data;
    return;
}
