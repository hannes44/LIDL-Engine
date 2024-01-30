project(Game, VERSION 0.01)

# Create library
add_library(gamelib
	TestGame.hpp
	TestGame.cpp
)
target_include_directories(gamelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gamelib PRIVATE editor)

# Create executable
add_executable(Game)
target_sources(Game PRIVATE main.cpp)

# Link libraries
target_link_libraries(Game PRIVATE
	editorlib
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(imguilib PRIVATE 
	imgui
	SDL3::SDL3
)
