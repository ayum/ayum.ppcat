target_sources(ppcat INTERFACE
    picker.cpp picker.hpp picker.test.cpp
)

target_include_directories(ppcat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

_pmm(cli11 mio fmt ICU nlohmann-json pegtl ayum)

find_package(ayum CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE ayum::dld)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE mio::mio mio::mio_base)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE fmt::fmt fmt::fmt-header-only)

find_package(ICU COMPONENTS uc data i18n REQUIRED)
target_link_libraries(ppcat INTERFACE ICU::i18n ICU::uc ICU::data)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE nlohmann_json nlohmann_json::nlohmann_json)

find_package(pegtl CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE taocpp::pegtl)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE doctest::doctest)
endif()
