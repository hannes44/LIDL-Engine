cmake_minimum_required(VERSION 3.10)

project(Engine, VERSION 0.01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Engine
	engine/main.cpp
	engine/transform.hpp
	engine/transform.cpp
	engine/game_object.hpp
	engine/game_object_manager.hpp
	engine/game_object_manager.cpp
)

add_subdirectory(external_src/glm)

target_link_libraries(Engine PUBLIC 
	glm
)