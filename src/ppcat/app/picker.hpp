#ifndef PICKER_HPP
#define PICKER_HPP

#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <filesystem>

namespace ppcat::app {

struct picker {
    struct config {
        std::string file;
    };

    picker(const config &config);

    nlohmann::json pick();

private:
    std::filesystem::path path;
};

}


#endif // PICKER_HPP
