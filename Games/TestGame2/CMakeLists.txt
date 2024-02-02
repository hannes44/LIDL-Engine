project(Game2, VERSION 0.01)

# Create executable
add_executable(Game2)
target_sources(Game2 PRIVATE 
	TestGame2.hpp
	TestGame2.cpp
)

add_library(game2lib 
	TestGame2.hpp
	TestGame2.cpp
)

target_include_directories(game2lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(Game2 PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(game2lib PRIVATE
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
