add_executable(app)
set_target_properties(app PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_sources(app PRIVATE
    main.cc
    main.cpp main.hpp main.test.cpp
    frontend.cpp frontend.hpp frontend.test.cpp
)

target_link_libraries(app PRIVATE ppcat)

_pmm(cli11 fmt ayum)

find_package(ayum CONFIG REQUIRED)
target_link_libraries(app PRIVATE ayum::dld)
if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(app PRIVATE ayum::dld-test)
endif()

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
