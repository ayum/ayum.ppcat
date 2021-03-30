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
gggggggg < ee >

автор
тт <мм>

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
                       {"subtitle", json::array({"подназвание1"})},
                       {"short_title", "подназвание2"},
                       {"title_slug", ""},
                       {"title", "название"},
                       {"body", json::array()}};
        CHECK(picker.pick(input) == output);
    }

    SUBCASE("first section article pishmo") {
        std::string_view input(R"(
письмо
название
подназвание1
подназвание2

)");
        json output = {{"type", "письмо"},
                       {"subtitle", json::array({"подназвание1"})},
                       {"short_title", "подназвание2"},
                       {"title_slug", ""},
                       {"title", "название"},
                       {"body", json::array()}};
        CHECK(picker.pick(input) == output);
    }

    SUBCASE("no article") {
        std::string_view input(R"(
название
подназвание1
подназвание2 < а б в г  >

 текст1
 текст2
)");
        json output = {{"type", ""},
                       {"subtitle", json::array({"подназвание1"})},
                       {"title_slug", "а б в г"},
                       {"short_title", "подназвание2"},
                       {"title", "название"},
                       {"body", json::array({" текст1\n текст2\n"})}};
        CHECK(picker.pick(input) == output);
    }

        SUBCASE("no article, summary, paragraph") {
            std::string_view input(R"(
название

описание
12345678
ddd

текст1
текст2
)");
            json output = {{"type", ""},
                           {"title", "название"},
                           {"subtitle", json::array()},
                           {"summary", json::array({"12345678", "ddd"})},
                           {"body", json::array({"текст1\nтекст2\n"})}};
            CHECK(picker.pick(input) == output);
        }

        SUBCASE("author, date, article") {
            std::string_view input(R"(
автор
сам <ссылка >

дата
01.01.2001 <12.12.1001>

статья
название)");
            json output = {{"type", "статья"},
                           {"title", "название"},
                           {"subtitle", json::array()},
                           {"body", json::array()},
                           {"date", "01.01.2001"},
                           {"publication_date", "12.12.1001"},
                           {"author", {{"name", "сам"}, {"link", "ссылка"}}}};
            CHECK(picker.pick(input) == output);
        }

        SUBCASE("author, date, no angles") {
            std::string_view input(R"(
автор
сам <>

дата
01.01.2001

)");
            json output = {{"date", "01.01.2001"},
                           {"publication_date", ""},
                           {"author", {{"name", "сам"}, {"link", ""}}}};
            CHECK(picker.pick(input) == output);
        }
}
