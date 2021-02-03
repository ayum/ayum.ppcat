cmake_minimum_required(VERSION 3.13)

project(ppcat
    VERSION 0.1.0
    DESCRIPTION "Typesetting workflow for articles"
    LANGUAGES CXX;C
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/tools/cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall\;-Wextra\;-Werror>)
add_compile_options($<$<CXX_COMPILER_ID:GNU>:-fdirectives-only>)
# Should be enabled when CLI11 library will support it
#add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fno-rtti>)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(SYSTEM external)

include(CTest)
include(FindPkgConfig)

option(PROJECT_USE_PMM "Autoinstall dependencies on configure with pmm" ON)
macro(_pmm)
if(PROJECT_USE_PMM)
pmm(VCPKG REVISION 2020.11-1 REQUIRES ${ARGV} PORTS
    "${CMAKE_SOURCE_DIR}/tools/vcpkg/ports/ayum"
)
endif()
endmacro()

if(PROJECT_USE_PMM)
set(PMM_URL_BASE "file://${CMAKE_CURRENT_LIST_DIR}/tools/pmm")
include(pmm.cmake)
endif()
if(BUILD_TESTING)
_pmm(doctest)
endif()

_pmm(ayum)
find_package(ayum CONFIG REQUIRED)
include(AyumCoverageHelper)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/opt/papercat" CACHE PATH "..." FORCE)
endif()

add_subdirectory(external)
add_subdirectory(src)
