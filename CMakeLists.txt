cmake_minimum_required(VERSION 3.10)

project(Engine, VERSION 0.01)

add_executable(Engine
	engine/main.cpp
	engine/transform.hpp
	engine/transform.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(external_src/glm)

target_link_libraries(Engine PUBLIC 
	glm
)