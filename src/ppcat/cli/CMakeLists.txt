add_executable(cli)
set_target_properties(cli PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_sources(cli PRIVATE
    main.cc
    main.cpp main.hpp main.test.cpp
    cli.cpp cli.hpp cli.test.cpp
    parser.cpp parser.hpp parser.test.cpp
    $<TARGET_OBJECTS:common>
)

target_link_libraries(cli PRIVATE common)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(cli PRIVATE CLI11::CLI11)

find_package(doctest CONFIG REQUIRED)
target_link_libraries(cli PRIVATE doctest::doctest)

add_test(NAME cli COMMAND cli tests)

install(TARGETS cli RUNTIME DESTINATION "bin")
