target_sources(ppcat INTERFACE
    cli.cpp cli.hpp
)

target_include_directories(ppcat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

_pmm(cli11)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE CLI11::CLI11)
