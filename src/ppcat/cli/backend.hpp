#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "picker.hpp"

#include <nlohmann/json.hpp>

#include <tuple>
#include <filesystem>

namespace ppcat::cli {

struct backend {

    using config = std::tuple<picker::config>;

    backend(const config &config);

    void run();

private:
    void run_once(std::filesystem::path path);

private:
    config _config;

};

}


#endif // BACKEND_HPP
