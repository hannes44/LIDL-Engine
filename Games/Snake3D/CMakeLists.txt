project(Snake3DExe, VERSION 0.01)

# Create executable
add_executable(Snake3DExe)
target_sources(Snake3DExe PRIVATE 
	Snake3D.hpp
	Snake3D.cpp
)

add_library(Snake3D SHARED
	Snake3D.hpp
	Snake3D.cpp
)

target_include_directories(Snake3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(Snake3DExe PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(Snake3D PRIVATE
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