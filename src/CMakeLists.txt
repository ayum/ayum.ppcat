add_compile_definitions(
    PROJECT_VERSION=\"${PROJECT_VERSION}\"
    PROJECT_DESCRIPTION=\"${PROJECT_DESCRIPTION}\"
)
if(NOT BUILD_TESTING)
add_compile_definitions(
   DOCTEST_CONFIG_DISABLE
)
endif()

if(BUILD_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
        find_program(COV_COMMAND gcov)
        if(NOT COV_COMMAND)
            message(FATAL_ERROR "Gcov cannot be found")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        find_program(COV_COMMAND llvm-cov)
        if(NOT COV_COMMAND)
            message(FATAL_ERROR "Llvm-cov cannot be found")
        endif()
        set(COV_COMMAND "${COV_COMMAND} gcov")
    else()
        message(FATAL_ERROR "Coverage cannot be enabled when compiler is not gcc or clang")
    endif()
    find_program(GCOVR_COMMAND gcovr)
    if(NOT GCOVR_COMMAND)
            message(FATAL_ERROR "Gcovr cannot be found")
    endif()
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage)
    add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:--coverage>)
    add_link_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:--coverage>)
    add_custom_target(coverage
        COMMAND ${GCOVR_COMMAND} --gcov-executable ${COV_COMMAND} --root ${CMAKE_CURRENT_SOURCE_DIR} --html --html-details --output ${CMAKE_CURRENT_BINARY_DIR}/coverage/coverage.html ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        )
endif()

add_subdirectory(ppcat)
