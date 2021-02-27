#include "picker.hpp"

#include <doctest/doctest.h>
#include <fmt/format.h>

#include <nlohmann/json.hpp>

#include <sstream>
#include <string_view>

using namespace ppcat::backend;
using namespace nlohmann;

TEST_CASE("picker text parsing") {
    ppcat::backend::picker picker{picker::config{}};

    SUBCASE("sandpit") {
        std::string_view input(R"(
название название
ddddddddd ddddddddd
gggggggg

hhhhhhh
88888

77777


* * *

)");
        json output = picker.pick(input);
        fmt::print("{}\n", output.dump());
    }

    SUBCASE("first section article") {
        std::string_view input(R"(
статья
название
подназвание1
подназвание2

)");
        json output = {{"type", "статья"},
                       {"subtitle", {"подназвание1", "подназвание2"}},
                       {"title", "название"},
                       {"body", json::array()}};
        CHECK(picker.pick(input) == output);
    }

    SUBCASE("no article") {
            std::string_view input(R"(
название
подназвание1
подназвание2

текст1
текст2
)");
            json output = {{"type", ""},
                           {"subtitle", {"подназвание1", "подназвание2"}},
                           {"title", "название"},
                           {"body", json::array({"текст1\nтекст2\n"})}};
            CHECK(picker.pick(input) == output);
    }

        SUBCASE("no article, summary, paragraph") {
            std::string_view input(R"(
название
подназвание1
подназвание2

описание
12345678
ddd

текст1
текст2
)");
            json output = {{"type", ""},
                           {"subtitle", {"подназвание1", "подназвание2"}},
                           {"title", "название"},
                           {"summary", "12345678\nddd\n"},
                           {"body", json::array({"текст1\nтекст2\n"})}};
            CHECK(picker.pick(input) == output);
    }
}
