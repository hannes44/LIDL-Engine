project(FPSGameExe, VERSION 0.01)

# Create executable
add_executable(FPSGameExe)
target_sources(FPSGameExe PRIVATE 
	FPSGame.hpp
	FPSGame.cpp
)

add_library(FPSGame SHARED
	FPSGame.hpp
	FPSGame.cpp
)

target_include_directories(FPSGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(FPSGameExe PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(FPSGame PRIVATE
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