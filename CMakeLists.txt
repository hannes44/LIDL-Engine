cmake_minimum_required(VERSION 3.20)

project(Engine, VERSION 0.01)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(vendor)
add_subdirectory(engine)
