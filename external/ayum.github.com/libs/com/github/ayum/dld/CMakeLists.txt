add_library(com.github.ayum.dld STATIC EXCLUDE_FROM_ALL
    dld.cpp dld.hpp dld.test.cpp
)

target_include_directories(com.github.ayum.dld INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(com.github.ayum.dld PRIVATE doctest::doctest)
endif()

add_library(ayum::dld ALIAS com.github.ayum.dld)
