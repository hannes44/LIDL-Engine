project(MultiplayerGameExe, VERSION 0.01)

# Create executable
add_executable(MultiplayerGameExe)
target_sources(MultiplayerGameExe PRIVATE 
	MultiplayerGame.hpp
	MultiplayerGame.cpp
)

add_library(MultiplayerGame SHARED
	MultiplayerGame.hpp
	MultiplayerGame.cpp
)

target_include_directories(MultiplayerGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(MultiplayerGameExe PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(MultiplayerGame PRIVATE
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