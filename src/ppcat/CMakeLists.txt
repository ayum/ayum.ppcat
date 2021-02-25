add_library(ppcat INTERFACE)

target_include_directories(ppcat INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(NOT BUILD_TESTING)
add_compile_definitions(
   DOCTEST_CONFIG_DISABLE
)
set(PROJECT_BUILD_TESTS "false")
else()
set(PROJECT_BUILD_TESTS "true")
endif()

configure_file(project.hpp.in project.hpp)

target_sources(ppcat INTERFACE
    "${CMAKE_CURRENT_BINARY_DIR}/project.hpp"
)

add_subdirectory(common)
add_subdirectory(engine/scripting)
add_subdirectory(backend)
