add_library(backend STATIC
    picker.cpp picker.hpp picker.test.cpp
)

target_include_directories(backend INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(backend PRIVATE common)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(backend PRIVATE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(backend PRIVATE mio::mio mio::mio_base)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(backend PRIVATE fmt::fmt fmt::fmt-header-only)

find_package(ICU COMPONENTS uc data REQUIRED)
target_link_libraries(backend PRIVATE ICU::uc ICU::data)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(backend PUBLIC nlohmann_json nlohmann_json::nlohmann_json)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(backend PRIVATE doctest::doctest)
endif()
