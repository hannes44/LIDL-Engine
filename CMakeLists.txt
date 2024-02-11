cmake_minimum_required(VERSION 3.20)

project(GameEngine, VERSION 0.01)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(vendor)
add_subdirectory(engine)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Lua/lua54.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LuaScripts/CompiledScripts/test.lua ${CMAKE_CURRENT_BINARY_DIR}/Debug/test.lua COPYONLY)

file(GLOB sources_list LIST_DIRECTORIES true ./Games/*)
foreach(dir ${sources_list})
    IF(IS_DIRECTORY ${dir})
        add_subdirectory(${dir})
    ELSE()
        CONTINUE()
    ENDIF()
endforeach()

add_subdirectory(editor)
