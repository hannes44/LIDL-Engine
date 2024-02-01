project(Editor, VERSION 0.01)

# Create executable
add_executable(Editor)
target_sources(Editor PRIVATE 
	main.cpp
	EditorGUI.hpp
	EditorGUI.cpp
	AssetManager.hpp
	AssetManager.cpp
	AssetNode.hpp
	EditorSerializer.hpp
	EditorSerializer.cpp
	EditorSettings.hpp
)

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
