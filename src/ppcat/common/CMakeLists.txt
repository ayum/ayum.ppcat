add_library(common OBJECT
    constants.cpp constants.hpp
    config.cpp config.hpp)
target_sources(common INTERFACE
    constants.hpp
    config.hpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
