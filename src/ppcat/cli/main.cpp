#include "constants.hpp"
#include "cli.hpp"

#include <iostream>
#include <fmt/format.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace ppcat;

namespace ppcat::cli {

int main(int argc, const char * const *argv) {
    cli app{argc, argv};

    if (int code = app.parse()) {
        return code;
    }

    if constexpr (common::build_tests) {
        if (app.tests()) {
            doctest::Context context;
            context.applyCommandLine(argc, argv);
            if (context.shouldExit()) {
                return 0;
            }
            return context.run();
        }
    }

    try {
        app.run();
    } catch(const std::exception& e) {
        std::cerr << fmt::format("{}", e.what()) << std::endl;
        return 1;
    }

    return 0;
}

}