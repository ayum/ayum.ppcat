cmake_minimum_required(VERSION 3.13)

project(ppcat
    VERSION 0.1.0
    DESCRIPTION "Typesetting workflow for articles"
    LANGUAGES CXX;C
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall\;-Wextra\;-Werror>)
# Should be enabled when CLI11 library will support it
#add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fno-rtti>)

include(CTest)
include(FindPkgConfig)

#pkg_search_module(dpdk REQUIRED IMPORTED_TARGET libdpdk)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/opt/papercat" CACHE PATH "..." FORCE)
endif()

add_subdirectory(src)
