add_library(scripting STATIC
    engine.cpp engine.hpp engine.startup.hpp engine.test.cpp
)

target_include_directories(scripting INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../)

target_link_libraries(scripting PRIVATE common)

_pmm(VCPKG REVISION 2020.11-1
    REQUIRES cli11 mio chaiscript
)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(scripting PRIVATE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(scripting PRIVATE mio::mio mio::mio_base)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(scripting PRIVATE doctest::doctest)
endif()
