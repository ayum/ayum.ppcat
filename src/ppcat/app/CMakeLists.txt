add_executable(app)
set_target_properties(app PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_sources(app PRIVATE
    main.cc
    main.cpp main.hpp main.test.cpp
    cli.cpp cli.hpp cli.test.cpp
    picker.cpp picker.hpp picker.test.cpp
    applicator.cpp applicator.hpp applicator.test.cpp
    backend.cpp backend.hpp backend.test.cpp
    $<TARGET_OBJECTS:common>
)

target_link_libraries(app PRIVATE common)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(app PRIVATE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(app PRIVATE mio::mio mio::mio_base)

find_package(inja CONFIG REQUIRED)
target_link_libraries(app PRIVATE pantor::inja)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(app PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(app PRIVATE fmt::fmt fmt::fmt-header-only)

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(app PRIVATE doctest::doctest)
endif()

add_test(NAME app COMMAND app tests)

install(TARGETS app RUNTIME DESTINATION "bin")
