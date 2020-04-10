#ifndef APPLICATOR_HPP
#define APPLICATOR_HPP

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include <string>
#include <filesystem>
#include <ostream>

namespace ppcat::app {

struct applicator {
    struct config {
        std::string templet;
    };

    applicator(const config &config);

    static void define_cli(CLI::App &app, config &config);
    void apply(std::ostream& os, const nlohmann::json &data);

private:
    std::filesystem::path path;
};

}


#endif // APPLICATOR_HPP
