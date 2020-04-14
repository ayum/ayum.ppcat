#include "cli.hpp"
#include "backend.hpp"
#include "picker.hpp"

#include <iostream>
#include <fmt/format.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace ppcat;

namespace ppcat::app {

int main(int argc, const char * const *argv) {
    common::cli::cli<backend::backend, backend::picker> app{argc, argv};

    if (int code = app.parse()) {
        return code;
    }

    if constexpr (common::cli::build_tests) {
        if (app.tests()) {
            doctest::Context context;
            context.applyCommandLine(argc, argv);
            return context.run();
        }
    }

    try {
        backend::backend(app.get_config()).run();
    } catch(const std::exception& e) {
        std::cerr << fmt::format("{}", e.what()) << std::endl;
        return 1;
    }

    return 0;
}

}