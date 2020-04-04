#ifndef PICKER_HPP
#define PICKER_HPP

#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <filesystem>

namespace ppcat::cli {

struct picker {
    struct config {
        std::vector<std::string> files;
    };

    picker(const std::filesystem::path &path);

    nlohmann::json pick();

private:
    std::filesystem::path path;
};

}


#endif // PICKER_HPP
