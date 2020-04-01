#include "main.hpp"

#include <doctest/doctest.h>

using namespace ppcat::cli;

template<typename T>
constexpr inline const T& tmp(const T& t) {
    return t;
}

TEST_CASE("testing tests option") {
    CHECK(main(3, tmp((const char *[]){"ppcat", "tests", "--help", nullptr})) == 0);
}

TEST_CASE("testing invalid options") {
    CHECK(main(2, tmp((const char *[]){"ppcat", "nonexistent-argument", nullptr})) == 109);
    CHECK(main(3, tmp((const char *[]){"ppcat", "nonexistent-argument", "tests", nullptr})) == 109);
}

TEST_CASE("testing valid options") {
    CHECK(main(2, tmp((const char *[]){"ppcat", "--help", nullptr})) == 0);
}
