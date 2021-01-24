add_library(aym STATIC
    dld.cpp dld.hpp dld.test.cpp
)

target_include_directories(aym INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(aym PRIVATE doctest::doctest)
endif()
