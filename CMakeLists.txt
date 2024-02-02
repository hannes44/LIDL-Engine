cmake_minimum_required(VERSION 3.20)

project(GameEngine, VERSION 0.01)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# DESPAIR, major warcrimes
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/Shaders DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(vendor)
add_subdirectory(engine)

file(GLOB sources_list LIST_DIRECTORIES true ./Games/*)
foreach(dir ${sources_list})
    IF(IS_DIRECTORY ${dir})
        add_subdirectory(${dir})
    ELSE()
        CONTINUE()
    ENDIF()
endforeach()

add_subdirectory(editor)