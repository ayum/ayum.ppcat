#include "picker.hpp"

using namespace ppcat::cli;
using namespace nlohmann;
using namespace std;

picker::picker(const std::filesystem::path &path)
    : path(path)
{
}

json picker::pick() {
    return {};
}
