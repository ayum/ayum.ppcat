add_library(backend OBJECT
    picker.cpp picker.hpp picker.test.cpp
    backend.cpp backend.hpp backend.test.cpp
)

target_include_directories(backend INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(backend PRIVATE common)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(backend PRIVATE CLI11::CLI11)

find_package(mio CONFIG REQUIRED)
target_link_libraries(backend PRIVATE mio::mio mio::mio_base)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(backend PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(backend PRIVATE fmt::fmt fmt::fmt-header-only)

find_path(ICU_INCLUDE_PATH unistr.h PATH_SUFFIXES unicode)
find_path(ICU_LIBRARY_PATH libicuuc.a PATH_SUFFIXES lib lib64 lib32)
if(ICU_INCLUDE_PATH AND ICU_LIBRARY_PATH)
target_include_directories(backend PRIVATE "${ICU_INCLUDE_PATH}")
target_link_directories(backend INTERFACE "${ICU_LIBRARY_PATH}")
target_link_libraries(backend PRIVATE icuuc dl icudata)
else()
message(FATAL_ERROR "ICU library not found")
endif()

if(BUILD_TESTING)
find_package(doctest CONFIG REQUIRED)
target_link_libraries(backend PRIVATE doctest::doctest)
endif()
