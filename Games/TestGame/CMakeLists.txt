project(Game, VERSION 0.01)

# Create executable
add_executable(Game)
target_sources(Game PRIVATE 
	TestGame.hpp
	TestGame.cpp
)

add_library(TestGame SHARED
	TestGame.hpp
	TestGame.cpp
)

target_include_directories(TestGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(Game PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(TestGame PRIVATE
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