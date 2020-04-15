if(CMAKE_JS_VERSION)
else()
    return()
endif()

add_library(napi SHARED)
set_target_properties(napi PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(napi PROPERTIES PREFIX "" SUFFIX ".node")
target_sources(napi PRIVATE
    main.cc
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:backend>
    ${CMAKE_JS_SRC}
)

target_link_libraries(napi PRIVATE ${CMAKE_JS_LIB})
target_include_directories(napi PRIVATE ${CMAKE_JS_INC})
target_compile_definitions(napi PRIVATE NAPI_VERSION=3)
