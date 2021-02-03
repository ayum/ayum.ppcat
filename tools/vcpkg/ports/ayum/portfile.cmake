if(NOT VCPKG_CMAKE_SYSTEM_NAME OR VCPKG_CMAKE_SYSTEM_NAME STREQUAL "windows" OR VCPKG_CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
    message(FATAL_ERROR "Currently ayum only supports Linux platforms")
endif()

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ayum/ayum.github.com
    REF 5ab10576fcc6535f13618dd8ca28d950c250ebe7
    SHA512 8f2e87e8fefed8052201c3e8c8c30304e23325e4a04239a39245fd77457acd76f1433af4d9c0fae04b50729db977b60f8fb8e1374c7484dbd2043633163d99d9
    HEAD_REF main
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_install_cmake()

if (NOT VCPKG_CMAKE_SYSTEM_NAME OR VCPKG_CMAKE_SYSTEM_NAME STREQUAL "windows" OR VCPKG_CMAKE_SYSTEM_NAME STREQUAL "WindowsStore") #Windows
    vcpkg_fixup_cmake_targets(CONFIG_PATH CMake)
else() #Linux/Unix/Darwin
    vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/ayum)
endif()

file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug"
    "${CURRENT_PACKAGES_DIR}/bin"
    "${CURRENT_PACKAGES_DIR}/lib"
)

# Handle copyright
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/ayum" RENAME copyright)
