project(Editor, VERSION 0.01)

# Create library
add_library(editorlib
	EditorGUI.hpp
	EditorGUI.cpp
	AssetManager.hpp
	AssetManager.cpp
	AssetNode.hpp
	EditorSerializer.hpp
	EditorSerializer.cpp
	EditorSettings.hpp
)
target_include_directories(editorlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create executable
add_executable(Editor)
target_sources(Editor PRIVATE main.cpp)

# Link libraries
target_link_libraries(Editor PRIVATE
	gamelib
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
