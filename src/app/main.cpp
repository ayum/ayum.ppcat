#include "cli.hpp"
#include "frontend.hpp"

#include <fmt/format.h>
#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

import project;

using namespace ppcat;

namespace app {

int main(int argc, const char* const* argv) {
    common::cli::cli<frontend::frontend_types> app{argc, argv};

    if (int code = app.parse()) {
        return code;
    }

    if constexpr (project_build_tests) {
        if (app.tests()) {
            doctest::Context context;
            context.applyCommandLine(argc, argv);
            return context.run();
        }
    }

    try {
        frontend(app.get_config()).run();
    } catch (const std::exception& e) {
        std::cerr << fmt::format("{}", e.what()) << std::endl;
        return 1;
    }

    return 0;
}

}  // namespace app