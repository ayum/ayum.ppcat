#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <filesystem>

namespace ppcat::cli {

struct backend {

    struct config {
        std::vector<std::string> files;
    };

    backend(const config &config);

    void run();

private:
    nlohmann::json run_once(std::filesystem::path path);

private:
    config _config;

};

}


#endif // BACKEND_HPP
