add_library(engine
	main.cpp
	Engine.hpp
	Core/Transform.hpp
	Core/Transform.cpp
	Core/GameObject.hpp
	Core/GameObject.cpp
	Core/Game.hpp
	Core/Game.cpp
	Core/Bootstrap.hpp
	Core/Bootstrap.cpp
	Core/Logger.hpp
	Core/Logger.cpp
	Core/Window.hpp
	Core/Window.cpp
	Core/InputEvent.hpp
	Core/InputEvent.cpp
	Core/InputListener.hpp
	Core/InputListener.cpp
	Core/InputSystem.hpp
	Core/InputSystem.cpp
	Core/InputFramework.hpp
	Core/InputFramework.cpp
	Core/Uuid.hpp
	Core/Uuid.cpp
	Core/Camera.hpp
	Core/Camera.cpp
	Core/Texture.hpp
	Core/Texture.cpp
	Core/Material.hpp
	Core/GameConfig.hpp
	Components/Component.hpp
	Components/PointLightComponent.hpp
	Components/PhysicsComponent.hpp
	Components/MeshComponent.hpp
	Components/MeshComponent.cpp
	Renderer/Console/Display.hpp
	Renderer/Console/DisplayTerminal.hpp
	Renderer/Console/DisplayTerminal.cpp
	Renderer/Renderer.hpp
	Renderer/Renderer.cpp
	Renderer/GraphicsAPI.hpp
	Renderer/Buffer.hpp
	Renderer/Buffer.cpp
	Renderer/Shader.hpp
	Renderer/Shader.cpp
	Renderer/Vertex.hpp
	Renderer/RendererSettings.hpp
	Renderer/OpenGL/OpenGLGraphicsAPI.hpp
	Renderer/OpenGL/OpenGLGraphicsAPI.cpp
	Renderer/OpenGL/OpenGLBuffer.hpp
	Renderer/OpenGL/OpenGLBuffer.cpp
	Renderer/OpenGL/OpenGLShader.hpp
	Renderer/OpenGL/OpenGLShader.cpp
	Renderer/OpenGL/OpenGLTexture.hpp
	Renderer/OpenGL/OpenGLTexture.cpp
	Serializer/GameSerializer.hpp
	Serializer/GameSerializer.cpp
	SDL/SDLWindow.hpp
	SDL/SDLWindow.cpp
	Physics/GamePhysics.hpp
	Physics/GamePhysics.cpp
	Utils/Utils.hpp
	Utils/Utils.cpp
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

source_group("Utils" FILES
	Utils/Utils.hpp
	Utils/Utils.cpp
)


source_group("Physics" FILES
	Physics/GamePhysics.hpp
	Physics/GamePhysics.cpp
)

source_group("SDL" FILES
	SDL/SDLWindow.hpp
	SDL/SDLWindow.cpp
)


source_group("Core" FILES
	Core/Transform.hpp
	Core/Transform.cpp
	Core/GameObject.hpp
	Core/GameObject.cpp
	Core/Game.hpp
	Core/Game.cpp
	Core/Bootstrap.hpp
	Core/Bootstrap.cpp
	Core/Logger.hpp
	Core/Logger.cpp
	Core/Window.hpp
	Core/Window.cpp
	Core/InputEvent.hpp
	Core/InputEvent.cpp
	Core/InputListener.hpp
	Core/InputListener.cpp
	Core/InputSystem.hpp
	Core/InputSystem.cpp
	Core/InputFramework.hpp
	Core/InputFramework.cpp
	Core/Uuid.hpp
	Core/Uuid.cpp
	Core/Utils.hpp
	Core/Utils.cpp
	Core/Camera.hpp
	Core/Camera.cpp
	Core/Texture.hpp
	Core/Texture.cpp
	Core/Material.hpp
	Core/GameConfig.hpp
)

source_group("Vendor" FILES
	../vendor/tinyobjloader/tiny_obj_loader.h
	../vendor/stb/stb_image.h
	../vendor/ImGuizmo/ImGuizmo.h
	../vendor/ImGuizmo/ImGuizmo.cpp
)


# Find *all* shaders.
file(GLOB_RECURSE SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.frag"
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
