#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string_view>

#ifdef DOCTEST_CONFIG_DISABLE
#define PROJECT_BUILD_TESTS 0
#else
#define PROJECT_BUILD_TESTS 1
#endif

namespace ppcat::common {
constexpr std::string_view version = PROJECT_VERSION;
constexpr std::string_view description = PROJECT_DESCRIPTION;
constexpr bool build_tests = PROJECT_BUILD_TESTS;
}

#endif /* CONSTANTS_HPP */
