#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <vector>
#include <string>

namespace ppcat::cli {

struct backend {

    struct config {
        std::vector<std::string> files;
    };

    backend(const config &config);

private:
    config _config;

};

}


#endif // BACKEND_HPP
