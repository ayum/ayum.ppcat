add_executable(app)
set_target_properties(app PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_sources(app PRIVATE
    main.cc
    main.cpp main.hpp main.test.cpp
    frontend.cpp frontend.hpp frontend.test.cpp
)

# --whole-archive needed for inclusion of doctest tests into release binary
target_link_libraries(app PRIVATE "-Wl,--whole-archive" common "-Wl,--no-whole-archive")
target_link_libraries(app PRIVATE "-Wl,--whole-archive" backend "-Wl,--no-whole-archive")
target_link_libraries(app PRIVATE "-Wl,--whole-archive" scripting "-Wl,--no-whole-archive")

_pmm(VCPKG REVISION 2020.11-1
    REQUIRES cli11 fmt ayum
    PORTS "${CMAKE_SOURCE_DIR}/tools/vcpkg/ports/ayum"
)

find_package(ayum CONFIG REQUIRED)
target_link_libraries(app PRIVATE ayum::dld)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(app PRIVATE CLI11::CLI11)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(app PRIVATE fmt::fmt fmt::fmt-header-only)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(app PRIVATE doctest::doctest)
endif()

add_test(NAME app COMMAND app tests)

install(TARGETS app RUNTIME)
