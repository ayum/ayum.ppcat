#include "constants.hpp"
#include "cli.hpp"

#include <CLI/CLI.hpp>

//
#include <fmt/format.h>

using namespace ppcat::cli;
using namespace std;

cli::cli(int &argc, const char * const *&argv)
    : app{std::string(ppcat::common::description)}
    , argc_orig{argc}, argv_orig{argv}
    , argc{argc}, argv{argv}
    , _config{}
{
    define();
}

int cli::parse() {
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::cerr << CLI::FailureMessage::simple(&app, e);
        return e.get_exit_code();
    }
    return 0;
}

void cli::define() {
    using namespace ppcat;
    app.set_help_flag();
    app.add_flag("-h,--help", [this](int){
        std::cout << std::flush << this->app.help() << std::endl << std::flush;
    }, "Print help message");
    app.add_flag("-v,--version", [](int) {
        std::cout << std::flush << common::version << std::endl << std::flush;
    }, "Print version");

    app.add_option("files", std::get<picker::config>(_config.get<backend::config>()).files, "Files to read template parameters from");

    if constexpr (common::build_tests) {
        auto &tests = *app.add_subcommand("tests", "Run tests");
        tests.prefix_command();
        tests.preparse_callback([this](int remain){
            this->argv_orig = this->argv_orig + (this->argc_orig - remain - 1);
            this->argc_orig = remain + 1;
        });
        tests.set_help_flag();
    }
}

bool cli::tests() const {
    return static_cast<bool>(*app.get_subcommand("tests"));
}


void cli::run() {
    backend(_config.get<backend::config>()).run();
}
