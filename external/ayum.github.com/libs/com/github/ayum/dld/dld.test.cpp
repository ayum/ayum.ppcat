#include "dld.hpp"

#include <doctest/doctest.h>

#include <utility>

using namespace com::github::ayum::dld;

TEST_CASE("testing transposition") {
    CHECK(damerau_levenshtein_distance("", "") == 0);
    CHECK(damerau_levenshtein_distance("a", "a") == 0);
    CHECK(damerau_levenshtein_distance("ab", "ba") == 1);
    CHECK(damerau_levenshtein_distance("aa", "aa") == 0);
    CHECK(damerau_levenshtein_distance("aba", "baa") == 1);
    CHECK(damerau_levenshtein_distance("aab", "baa") == 2);
    CHECK(damerau_levenshtein_distance("aaab", "baaa") == 2);
    CHECK(damerau_levenshtein_distance("abab", "baba") == 2);
}

TEST_CASE("testing insertions, deletions, edits") {
    CHECK(damerau_levenshtein_distance("", "a") == 1);
    CHECK(damerau_levenshtein_distance("b", "abb") == 2);
    CHECK(damerau_levenshtein_distance("rosettacode", "raisethysword") == 8);
}
