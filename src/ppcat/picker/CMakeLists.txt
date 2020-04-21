add_library(picker STATIC
    picker.cpp picker.hpp picker.test.cpp
)

target_include_directories(picker INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(picker PRIVATE common)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(picker PRIVATE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(picker PRIVATE mio::mio mio::mio_base)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(picker PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(picker PRIVATE fmt::fmt fmt::fmt-header-only)

find_package(ICU COMPONENTS uc data REQUIRED)
target_link_libraries(picker PRIVATE ICU::uc ICU::data)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(picker PRIVATE doctest::doctest)
endif()
