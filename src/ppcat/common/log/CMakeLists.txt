target_sources(ppcat INTERFACE
    logging.cpp logging.hpp
)

target_include_directories(ppcat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

_pmm(spdlog fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE spdlog::spdlog_header_only)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE fmt::fmt fmt::fmt-header-only)
