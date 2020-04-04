add_executable(cli)
set_target_properties(cli PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_sources(cli PRIVATE
    main.cc
    main.cpp main.hpp main.test.cpp
    cli.cpp cli.hpp cli.test.cpp
    reaper.cpp reaper.hpp reaper.test.cpp
#    applier.cpp applier.hpp applier.test.cpp
    backend.cpp backend.hpp backend.test.cpp
    $<TARGET_OBJECTS:common>
)

target_link_libraries(cli PRIVATE common)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(cli PRIVATE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(cli PRIVATE mio::mio mio::mio_base)

find_package(inja CONFIG REQUIRED)
target_link_libraries(cli PRIVATE pantor::inja)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(cli PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(cli PRIVATE fmt::fmt fmt::fmt-header-only)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(cli PRIVATE doctest::doctest)
endif()

add_test(NAME cli COMMAND cli tests)

install(TARGETS cli RUNTIME DESTINATION "bin")
