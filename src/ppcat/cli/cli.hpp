#ifndef CLI_HPP
#define CLI_HPP

#include "config.hpp"

#include <CLI/CLI.hpp>

#include <string>
#include <vector>

namespace ppcat::cli {

using config = ppcat::common::config<>;

struct cli {
    cli(int &argc, const char * const *&argv);

    int parse();
    bool tests() const;
    config &get_config() const;

private:
    void define();

private:
    CLI::App app;
    int &argc_orig;
    const char * const *&argv_orig;
    const int argc;
    const char * const *argv;
    config _config;
};

}

#endif /* CLI_HPP */
