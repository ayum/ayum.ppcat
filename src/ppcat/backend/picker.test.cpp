#include "picker.hpp"

#include <doctest/doctest.h>

#include <nlohmann/json.hpp>

#include <string_view>

using namespace ppcat::backend;
using namespace nlohmann;

TEST_CASE("picker text parsing") {
    ppcat::backend::picker picker{picker::config{}};

    SUBCASE("several keys in one chunk") {
        std::string_view input(R"(
<ключ>
<ключ1>: значение 1
<ключ2>: значение 2
<ключ3>: значение 3)"
        );

        json output = {
            {"ключ", {{
                {"ключ1", "значение 1"},
                {"ключ2", "значение 2"},
                {"ключ3", "значение 3"},
            }}}
        };
        CHECK(picker.pick(input) == output);
    }
    SUBCASE("multiline value") {
        std::string_view input(R"(
<ключ>:
строка 1
строка 2)"
        );

        json output = {
            {"ключ", "строка 1\nстрока 2"}
        };
        CHECK(picker.pick(input) == output);
    }
    SUBCASE("slug key") {
        std::string_view input(R"(
<ключ 1 >:
строка 1
строка 2)"
        );

        json output = {
            {"ключ_1", "строка 1\nстрока 2"}
        };
        CHECK(picker.pick(input) == output);
    }
}