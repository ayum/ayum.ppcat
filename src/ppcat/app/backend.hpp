#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "picker.hpp"
#include "applicator.hpp"

#include <nlohmann/json.hpp>

#include <tuple>
#include <filesystem>

namespace ppcat::app {

struct backend {

    struct settings {
        struct config {
            std::string output;
        };
    };

    using config = std::tuple<picker::config, applicator::config>;

    backend(const config &config);

    void run();

private:
    void run_once(std::filesystem::path output);

private:
    config _config;

};

}


#endif // BACKEND_HPP
