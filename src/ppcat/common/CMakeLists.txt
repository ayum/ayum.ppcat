add_library(common OBJECT
    constants.cpp constants.hpp
    config.cpp config.hpp
    log/logging.cpp log/logging.hpp)
target_sources(common INTERFACE
    constants.hpp
    config.hpp
    log/logging.hpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/log)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(common PUBLIC spdlog::spdlog spdlog::spdlog_header_only)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(common PRIVATE fmt::fmt fmt::fmt-header-only)
