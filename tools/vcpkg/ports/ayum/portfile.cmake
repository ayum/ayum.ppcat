if(NOT VCPKG_CMAKE_SYSTEM_NAME OR VCPKG_CMAKE_SYSTEM_NAME STREQUAL "windows" OR VCPKG_CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
    message(FATAL_ERROR "Currently ayum only supports Linux platforms")
endif()

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ayum/ayum.github.com
    REF 6b987d883901afed9b08dc6a69df757916fabc23
    SHA512 52fa8f5548f61344c2135f5f67bfba75efb168d7620fabd86472b76a90a12d08dab1010dea99fad2b41f816712e5020df4c9c9d685c5f7addf942707632d2a14
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
    vcpkg_fixup_cmake_targets(CONFIG_PATH share/ayum)
endif()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include ${CURRENT_PACKAGES_DIR}/debug/share)

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/ayum RENAME copyright)
