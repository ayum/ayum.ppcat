#include "cli.hpp"
#include "frontend.hpp"

#include <iostream>
#include <fmt/format.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace ppcat;

namespace ppcat::app {

int main(int argc, const char * const *argv) {
    common::cli::cli<frontend::frontend_types> app{argc, argv};

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
        frontend(app.get_config()).run();
    } catch(const std::exception& e) {
        std::cerr << fmt::format("{}", e.what()) << std::endl;
        return 1;
    }

    return 0;
}

}