add_executable(Engine)

source_group("Components" FILES
	Component.hpp
	PointLightComponent.hpp
	PhysicsComponent.hpp
	MeshComponent.hpp
	MeshComponent.cpp
)

source_group("Renderer" FILES
	Renderer.hpp
	Renderer.cpp
	GraphicsAPI.hpp
	Buffer.hpp
	Buffer.cpp
	Shader.hpp
	Shader.cpp
	RendererSettings.hpp
)

source_group("OpenGL" FILES
	OpenGLGraphicsAPI.hpp
	OpenGLGraphicsAPI.cpp
	OpenGLShader.hpp
	OpenGLShader.cpp
	OpenGLBuffer.hpp
	OpenGLBuffer.cpp
	OpenGLTexture.hpp
	OpenGLTexture.cpp
)


source_group("Serializer" FILES
	GameSerializer.hpp
	GameSerializer.cpp
)

source_group("SDL" FILES
	SDLWindow.hpp
	SDLWindow.cpp
)

source_group("Editor" FILES
	EditorGUI.hpp
	EditorGUI.cpp
)

# Find *all* shaders.
file(GLOB_RECURSE SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
)
# Separate filter for shaders.
source_group("Shaders" FILES ${SHADERS})

target_sources(Engine PRIVATE 
	main.cpp
	Transform.hpp
	Transform.cpp
	GameObject.hpp
	GameObject.cpp
	GameObjectManager.hpp
	GameObjectManager.cpp
	Component.hpp
	PointLightComponent.hpp
	PhysicsComponent.hpp
	Game.hpp
	Game.cpp
	TestGame.hpp
	TestGame.cpp
	Display.hpp
	DisplayTerminal.hpp
	DisplayTerminal.cpp
	Bootstrap.hpp
	Bootstrap.cpp
	Logger.hpp
	Logger.cpp
	Window.hpp
	Window.cpp
	MeshComponent.hpp
	MeshComponent.cpp
	Vertex.hpp
	InputEvent.hpp
	InputEvent.cpp
	InputListener.hpp
	InputListener.cpp
	InputSystem.hpp
	InputSystem.cpp
	InputFramework.hpp
	InputFramework.cpp
	Cube.hpp
	Cube.cpp
	Renderer.hpp
	Renderer.cpp
	GraphicsAPI.hpp
	OpenGLGraphicsAPI.hpp
	OpenGLGraphicsAPI.cpp
	OpenGLBuffer.hpp
	OpenGLBuffer.cpp
	OpenGLShader.hpp
	OpenGLShader.cpp
	OpenGLTexture.hpp
	OpenGLTexture.cpp
	Buffer.hpp
	Buffer.cpp
	Shader.hpp
	Shader.cpp
	Utils.hpp
	Utils.cpp
	Camera.hpp
	Camera.cpp
	Texture.hpp
	Texture.cpp
	Material.hpp
	GameConfig.hpp
	GameSerializer.hpp
	GameSerializer.cpp
	SDLWindow.hpp
	SDLWindow.cpp
	EditorGUI.hpp
	EditorGUI.cpp
	Uuid.hpp
	Uuid.cpp
	RendererSettings.hpp
	../vendor/tinyobjloader/tiny_obj_loader.h
	../vendor/stb/stb_image.h
	../vendor/ImGuizmo/ImGuizmo.h
	../vendor/ImGuizmo/ImGuizmo.cpp
	${SHADERS}
)

target_link_libraries(Engine PRIVATE 
	imgui
	imguilib
	SDL3::SDL3
	vendor
)

target_link_libraries(imguilib PRIVATE 
	imgui
	SDL3::SDL3
)
