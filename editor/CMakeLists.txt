project(Editor, VERSION 0.01)

# Create executable
add_executable(Editor)
target_sources(Editor PRIVATE 
	src/main.cpp
	src/Serializer/EditorSerializer.hpp
	src/Serializer/EditorSerializer.cpp
	src/GUI/EditorGUI.hpp
	src/GUI/EditorGUI.cpp
	src/GUI/MainMenuGUI.hpp
	src/GUI/MainMenuGUI.cpp
	src/GUI/GUIHelper.hpp
	src/GUI/GUIHelper.cpp
	src/AssetManager.hpp
	src/AssetManager.cpp
	src/AssetNode.hpp
	src/EditorSettings.hpp
	src/Editor.hpp
	src/Editor.cpp
	src/Project.hpp
)

source_group("GUI" FILES
	src/GUI/EditorGUI.hpp
	src/GUI/EditorGUI.cpp
	src/GUI/MainMenuGUI.hpp
	src/GUI/MainMenuGUI.cpp
	src/GUI/GUIHelper.hpp
	src/GUI/GUIHelper.cpp
)

source_group("Serializer" FILES
	src/Serializer/EditorSerializer.hpp
	src/Serializer/EditorSerializer.cpp
)

target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
