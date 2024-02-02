add_library(engine
	main.cpp
	Engine.hpp
	Transform.hpp
	Transform.cpp
	GameObject.hpp
	GameObject.cpp
	Components/Component.hpp
	Components/PointLightComponent.hpp
	Components/PhysicsComponent.hpp
	Components/MeshComponent.hpp
	Components/MeshComponent.cpp
	Game.hpp
	Game.cpp
	Renderer/Console/Display.hpp
	Renderer/Console/DisplayTerminal.hpp
	Renderer/Console/DisplayTerminal.cpp
	Renderer/Vertex.hpp
	Bootstrap.hpp
	Bootstrap.cpp
	Logger.hpp
	Logger.cpp
	Window.hpp
	Window.cpp
	InputEvent.hpp
	InputEvent.cpp
	InputListener.hpp
	InputListener.cpp
	InputSystem.hpp
	InputSystem.cpp
	InputFramework.hpp
	InputFramework.cpp
	Renderer/Renderer.hpp
	Renderer/Renderer.cpp
	Renderer/GraphicsAPI.hpp
	Renderer/Buffer.hpp
	Renderer/Buffer.cpp
	Renderer/Shader.hpp
	Renderer/Shader.cpp
	Renderer/RendererSettings.hpp
	Renderer/OpenGL/OpenGLGraphicsAPI.hpp
	Renderer/OpenGL/OpenGLGraphicsAPI.cpp
	Renderer/OpenGL/OpenGLBuffer.hpp
	Renderer/OpenGL/OpenGLBuffer.cpp
	Renderer/OpenGL/OpenGLShader.hpp
	Renderer/OpenGL/OpenGLShader.cpp
	Renderer/OpenGL/OpenGLTexture.hpp
	Renderer/OpenGL/OpenGLTexture.cpp
	Utils.hpp
	Utils.cpp
	Camera.hpp
	Camera.cpp
	Texture.hpp
	Texture.cpp
	Material.hpp
	GameConfig.hpp
	Serializer/GameSerializer.hpp
	Serializer/GameSerializer.cpp
	SDL/SDLWindow.hpp
	SDL/SDLWindow.cpp
	Uuid.hpp
	Uuid.cpp
	Physics/GamePhysics.hpp
	Physics/GamePhysics.cpp
	../vendor/tinyobjloader/tiny_obj_loader.h
	../vendor/stb/stb_image.h
	../vendor/ImGuizmo/ImGuizmo.h
	../vendor/ImGuizmo/ImGuizmo.cpp
	${SHADERS}
)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

source_group("Components" FILES
	Components/Component.hpp
	Components/PointLightComponent.hpp
	Components/PhysicsComponent.hpp
	Components/MeshComponent.hpp
	Components/MeshComponent.cpp
)

source_group("Renderer" FILES
	Renderer/Renderer.hpp
	Renderer/Renderer.cpp
	Renderer/GraphicsAPI.hpp
	Renderer/Buffer.hpp
	Renderer/Buffer.cpp
	Renderer/Shader.hpp
	Renderer/Shader.cpp
	Renderer/RendererSettings.hpp
	Renderer/Vertex.hpp
)

source_group("OpenGL" FILES
	Renderer/OpenGL/OpenGLGraphicsAPI.hpp
	Renderer/OpenGL/OpenGLGraphicsAPI.cpp
	Renderer/OpenGL/OpenGLBuffer.hpp
	Renderer/OpenGL/OpenGLBuffer.cpp
	Renderer/OpenGL/OpenGLShader.hpp
	Renderer/OpenGL/OpenGLShader.cpp
	Renderer/OpenGL/OpenGLTexture.hpp
	Renderer/OpenGL/OpenGLTexture.cpp
)

source_group("Serializer" FILES
	Serializer/GameSerializer.hpp
	Serializer/GameSerializer.cpp
)

source_group("Physics" FILES
	Physics/GamePhysics.hpp
	Physics/GamePhysics.cpp
)

source_group("SDL" FILES
	SDL/SDLWindow.hpp
	SDL/SDLWindow.cpp
)

# Find *all* shaders.
file(GLOB_RECURSE SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
)
# Separate filter for shaders.
source_group("Shaders" FILES ${SHADERS})

# Link libraries
target_link_libraries(engine PRIVATE 
	vendor	
	imgui
	imguilib
	SDL3::SDL3
)

target_link_libraries(imguilib PRIVATE 
	imgui
	SDL3::SDL3
)
