add_library(common STATIC
    cli/cli.cpp cli/cli.hpp
    log/logging.cpp log/logging.hpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/log ${CMAKE_CURRENT_SOURCE_DIR}/cli)

_pmm(VCPKG REVISION 2020.11-1
    REQUIRES spdlog fmt
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(common PRIVATE spdlog::spdlog_header_only)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(common PRIVATE fmt::fmt fmt::fmt-header-only)
