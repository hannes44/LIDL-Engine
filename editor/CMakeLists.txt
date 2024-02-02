project(Editor, VERSION 0.01)

# Create executable
add_executable(Editor)
target_sources(Editor PRIVATE 
	main.cpp
	Serializer/EditorSerializer.hpp
	Serializer/EditorSerializer.cpp
	GUI/EditorGUI.hpp
	GUI/EditorGUI.cpp
	GUI/MainMenuGUI.hpp
	GUI/MainMenuGUI.cpp
	GUI/GUIHelper.hpp
	GUI/GUIHelper.cpp
	AssetManager.hpp
	AssetManager.cpp
	AssetNode.hpp
	EditorSettings.hpp
	Editor.hpp
	Editor.cpp
	Project.hpp
)

source_group("GUI" FILES
	GUI/EditorGUI.hpp
	GUI/EditorGUI.cpp
	GUI/MainMenuGUI.hpp
	GUI/MainMenuGUI.cpp
	GUI/GUIHelper.hpp
	GUI/GUIHelper.cpp
)

source_group("Serializer" FILES
	Serializer/EditorSerializer.hpp
	Serializer/EditorSerializer.cpp
)

target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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
