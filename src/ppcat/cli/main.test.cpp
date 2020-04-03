#include "main.hpp"

#include <doctest/doctest.h>

#include <utility>

using namespace ppcat::cli;

TEST_CASE("testing tests option") {
    CHECK(main(3, std::move((const char *[]){"ppcat", "tests", "--help", nullptr})) == 0);
}

TEST_CASE("testing invalid options") {
    CHECK(main(2, std::move((const char *[]){"ppcat", "nonexistent-argument", nullptr})) == 109);
    CHECK(main(3, std::move((const char *[]){"ppcat", "nonexistent-argument", "tests", nullptr})) == 109);
}

TEST_CASE("testing valid options") {
    CHECK(main(2, std::move((const char *[]){"ppcat", "--help", nullptr})) == 0);
}
