if(CMAKE_JS_VERSION)
else()
    return()
endif()

add_library(napi SHARED)
set_target_properties(napi PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(napi PROPERTIES PREFIX "" SUFFIX ".node")
target_sources(napi PRIVATE
    main.cc
    ${CMAKE_JS_SRC}
)

target_link_libraries(napi PRIVATE common)
target_link_libraries(napi PRIVATE backend)

execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(napi PRIVATE ${NODE_ADDON_API_DIR})

target_link_libraries(napi PRIVATE ${CMAKE_JS_LIB})
target_include_directories(napi PRIVATE ${CMAKE_JS_INC})
target_compile_definitions(napi PRIVATE NAPI_VERSION=3)
