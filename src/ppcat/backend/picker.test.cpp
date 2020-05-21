#include "picker.hpp"

#include <doctest/doctest.h>

#include <nlohmann/json.hpp>

#include <sstream>
#include <string_view>

using namespace ppcat::backend;
using namespace nlohmann;

TEST_CASE("picker text parsing") {
    ppcat::backend::picker picker{picker::config{}};

    SUBCASE("several keys in one chunk") {
        std::string_view input(R"(
<ключ>
<ключ1> значение 1
<ключ2> значение 2
<ключ3> значение 3)");

        json output = {{"ключ",
                        {
                            {"ключ1", "значение 1"},
                            {"ключ2", "значение 2"},
                            {"ключ3", "значение 3"},
                        }}};
        CHECK(picker.pick(input) == output);
    }
    SUBCASE("multiline value") {
        std::string_view input(R"(
<корень>
<ключ>
строка 1
строка 2)");

        json output = {{"корень", {{"ключ", "\nстрока 1\nстрока 2"}}}};
        CHECK(picker.pick(input) == output);
    }
    SUBCASE("slug key") {
        std::string_view input(R"(
<корень>
<клюЧ 1  ΣσςΣ > строка 1
строка 2)");

        json output = {{"корень", {{"ключ_1_σσςς", "строка 1\nстрока 2"}}}};
        CHECK(picker.pick(input) == output);
    }
}

TEST_CASE("picker pegtl streams") {
    ppcat::backend::picker picker{picker::config{}};
    SUBCASE("test1") {
        std::istringstream input(R"(
<статья>
<аттрибут> значение

параграф

<раздел 0>
<подраздел>

параграф подраздела

<раздел>

параграф раздела

<раздел 0>
<подраздел>

параграф подраздела следующего раздела
)");
        json output = {
            {"статья",
             {{"аттрибут", "значение"},
              {"paragraph", {"параграф"}},
              {"раздел",
               {{{"paragraph", {"параграф раздела"}},
                 {"подраздел", {{"paragraph", {"параграф подраздела"}}}}},
                {{"подраздел",
                  {{"paragraph",
                    {"параграф подраздела следующего раздела"}}}}}}}}}};
        CHECK(picker.pick(input) == output);
    }
}
