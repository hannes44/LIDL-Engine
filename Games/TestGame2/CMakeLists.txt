project(TestGame2Exe, VERSION 0.01)

# Create executable
add_executable(TestGame2Exe)
target_sources(TestGame2Exe PRIVATE 
	TestGame2.hpp
	TestGame2.cpp
)

add_library(TestGame2 SHARED
	TestGame2.hpp
	TestGame2.cpp
)

target_include_directories(TestGame2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(TestGame2Exe PRIVATE
	engine
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(TestGame2 PRIVATE
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
