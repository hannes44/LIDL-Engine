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
)

source_group("OpenGL" FILES
	OpenGLGraphicsAPI.hpp
	OpenGLGraphicsAPI.cpp
	OpenGLShader.hpp
	OpenGLShader.cpp
	OpenGLBuffer.hpp
	OpenGLBuffer.cpp
	OpenGLTexture.hpp
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
	Buffer.hpp
	Buffer.cpp
	Shader.hpp
	Shader.cpp
	Utils.hpp
	Utils.cpp
	Camera.hpp
	Camera.cpp
	Texture.hpp
	../vendor/tinyobjloader/tiny_obj_loader.h
)
target_link_libraries(Engine PRIVATE vendor)