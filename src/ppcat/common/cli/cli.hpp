#ifndef CLI_HPP
#define CLI_HPP

#include "logging.hpp"

#include <CLI/CLI.hpp>

#include <filesystem>
#include <string>
#include <tuple>

#ifdef DOCTEST_CONFIG_DISABLE
#define PROJECT_BUILD_TESTS 0
#else
#define PROJECT_BUILD_TESTS 1
#endif

namespace ppcat::common::cli {

constexpr std::string_view version = PROJECT_VERSION;
constexpr std::string_view description = PROJECT_DESCRIPTION;
constexpr bool build_tests = PROJECT_BUILD_TESTS;

template <typename X>
struct cli;

template <typename... T>
struct cli<std::tuple<T...>> {
    template <typename S>
    using config_type = S::config;

    using config_t = std::tuple<config_type<T>...>;

    cli(int &argc, const char *const *&argv)
        : app{std::string(description)},
          argc_orig{argc},
          argv_orig{argv},
          argc{argc},
          argv{argv},
          config{} {
        define();
    }

    int parse() {
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError &e) {
            std::cerr << CLI::FailureMessage::simple(&app, e);
            return e.get_exit_code();
        }
        return 0;
    }

    bool tests() const {
        return static_cast<bool>(*app.get_subcommand("tests"));
    }
    const config_t &get_config() const { return config; }

   private:
    template <typename S>
    void define_one() {
        S::define_cli(app, std::get<config_type<S>>(config));
    }

    void define() {
        app.option_defaults()->always_capture_default();

        app.set_help_flag();
        app.add_flag(
            "-h,--help",
            [this](int) {
                std::cout << std::flush << this->app.help() << std::endl
                          << std::flush;
            },
            "Print help message");
        app.add_flag(
            "-v,--version",
            [](int) {
                std::cout << std::flush << version << std::endl << std::flush;
            },
            "Print version");

        log::set_level();
        app.add_option_function<std::string>(
            "-l,--log-level",
            [](const std::string &option) {
                if (not option.empty()) {
                    log::set_level(option);
                }
            },
            "Log level for logging, equal or severer wil be printed");

        app.add_option_function<std::string>(
            "-f,--log-file",
            [](const std::string &option) {
                if (not option.empty()) {
                    log::set_file(std::filesystem::path{option});
                }
            },
            "Log file for logging, stdout if empty");

        if constexpr (build_tests) {
            auto &tests = *app.add_subcommand("tests", "Run tests");
            tests.prefix_command();
            tests.preparse_callback([this](int remain) {
                this->argv_orig =
                    this->argv_orig + (this->argc_orig - remain - 1);
                this->argc_orig = remain + 1;
            });
            tests.set_help_flag();
        }

        (define_one<T>(), ...);
    }

   private:
    CLI::App app;
    int &argc_orig;
    const char *const *&argv_orig;
    const int argc;
    const char *const *argv;
    config_t config;
};

}  // namespace ppcat::common::cli

#endif /* CLI_HPP */
