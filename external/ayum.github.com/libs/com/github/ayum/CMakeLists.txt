if(BUILD_TESTING AND NOT hasParent)
pmm(VCPKG REVISION 2020.11-1 REQUIRES doctest)
endif()

add_subdirectory(dld)

if(BUILD_TESTING AND NOT hasParent)

add_executable(ayum-test main.cc)

target_link_libraries(ayum-test PRIVATE "-Wl,--whole-archive" ayum::dld "-Wl,--no-whole-archive")

find_package(doctest CONFIG REQUIRED)
target_link_libraries(ayum-test PRIVATE doctest::doctest)

endif()
