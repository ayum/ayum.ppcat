#include "picker.hpp"

#include <doctest/doctest.h>
#include <fmt/format.h>

#include <nlohmann/json.hpp>

#include <sstream>
#include <string_view>

using namespace ppcat::backend;
using namespace nlohmann;


namespace ppcat::backend::internal {

std::string make_slug(std::string_view input);

}

TEST_CASE("picker text parsing") {
    ppcat::backend::picker picker{picker::config{}};

    SUBCASE("several keys in one chunk") {
        std::string_view input(R"(  fer  m<<« NNNNN_N)NNNNNN Глава №1   

        dsfsdf

        ddddd         dfdfd
        dddd
        fff

        fffffs2




        )");
        json output = picker.pick(input);
        fmt::print("{}\n", output.dump());
        fmt::print("{}\n", ppcat::backend::internal::make_slug(output[0][0].dump()));
    }
}
