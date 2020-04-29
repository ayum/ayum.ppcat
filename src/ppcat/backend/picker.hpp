#ifndef PICKER_HPP
#define PICKER_HPP

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <istream>
#include <string_view>
#include <utility>

namespace ppcat::backend {

struct picker {
    struct config {
        std::size_t buffer_size = 1024 * 1024;
    };

    picker(const config &config);

    static void define_cli(CLI::App &app, config &config);
    nlohmann::json pick(std::string_view input);
    nlohmann::json pick(std::istream &input);

   private:
    std::size_t buffer_size;
};

}  // namespace ppcat::backend

#endif  // PICKER_HPP
