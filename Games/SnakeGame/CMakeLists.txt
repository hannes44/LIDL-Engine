project(Snake, VERSION 0.01)

# Create executable
add_executable(Snake)
target_sources(Snake PRIVATE 
	SnakeGame.hpp
	SnakeGame.cpp
)

add_library(SnakeGame SHARED
	SnakeGame.hpp
	SnakeGame.cpp
)

target_include_directories(SnakeGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(Snake PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(SnakeGame PRIVATE
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