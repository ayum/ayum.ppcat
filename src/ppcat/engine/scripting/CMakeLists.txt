target_sources(ppcat INTERFACE
    engine.cpp engine.hpp engine.startup.hpp engine.test.cpp
)

target_include_directories(ppcat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../")

_pmm(cli11 mio chaiscript)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE mio::mio mio::mio_base)

# the include dir already in path after installation with pmm
# find_path(CHAISCRIPT_INCLUDE_DIRS "chaiscript/chaiscript.hpp")
# target_include_directories(main PRIVATE ${CHAISCRIPT_INCLUDE_DIRS})

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(ppcat INTERFACE doctest::doctest)
endif()
