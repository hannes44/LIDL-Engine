add_library(engine
	src/main.cpp
	src/Engine.hpp
	src/Core/Transform.hpp
	src/Core/Transform.cpp
	src/Core/GameObject.hpp
	src/Core/GameObject.cpp
	src/Core/Game.hpp
	src/Core/Game.cpp
	src/Core/Bootstrap.hpp
	src/Core/Bootstrap.cpp
	src/Core/Logger.hpp
	src/Core/Logger.cpp
	src/Core/Window.hpp
	src/Core/Window.cpp
	src/Core/Uuid.hpp
	src/Core/Uuid.cpp
	src/Core/Texture.hpp
	src/Core/Texture.cpp
	src/Core/Material.hpp
	src/Core/GameConfig.hpp
	src/Core/BoundingBox.hpp
	src/Core/BoundingBox.cpp
	src/Core/Debug.hpp
	src/Core/Debug.cpp
	src/Core/ResourceManager.hpp
	src/Core/ResourceManager.cpp
	src/Components/Component.hpp
	src/Components/PointLightComponent.hpp
	src/Components/PhysicsComponent.hpp
	src/Components/MeshComponent.hpp
	src/Components/MeshComponent.cpp
	src/Components/ColliderComponent.hpp
	src/Components/ColliderComponent.cpp
	src/Components/BoxColliderComponent.hpp
	src/Components/BoxColliderComponent.cpp
	src/Components/SphereColliderComponent.hpp
	src/Components/SphereColliderComponent.cpp
	src/Components/CameraComponent.hpp
	src/Components/CameraComponent.cpp
	src/Components/ComponentFactory.hpp
	src/Components/ComponentFactory.cpp
	src/Components/ControllableComponent.hpp
	src/Components/ControllableComponent.cpp
	src/Components/ScriptableComponent.hpp
	src/Components/ScriptableComponent.cpp
	src/Renderer/Console/Display.hpp
	src/Renderer/Console/DisplayTerminal.hpp
	src/Renderer/Console/DisplayTerminal.cpp
	src/Renderer/Renderer.hpp
	src/Renderer/Renderer.cpp
	src/Renderer/GraphicsAPI.hpp
	src/Renderer/Buffer.hpp
	src/Renderer/Buffer.cpp
	src/Renderer/Shader.hpp
	src/Renderer/Shader.cpp
	src/Renderer/Vertex.hpp
	src/Renderer/RendererSettings.hpp
	src/Renderer/OpenGL/OpenGLGraphicsAPI.hpp
	src/Renderer/OpenGL/OpenGLGraphicsAPI.cpp
	src/Renderer/OpenGL/OpenGLBuffer.hpp
	src/Renderer/OpenGL/OpenGLBuffer.cpp
	src/Renderer/OpenGL/OpenGLShader.hpp
	src/Renderer/OpenGL/OpenGLShader.cpp
	src/Renderer/OpenGL/OpenGLTexture.hpp
	src/Renderer/OpenGL/OpenGLTexture.cpp
	src/Serializer/GameSerializer.hpp
	src/Serializer/GameSerializer.cpp
	src/Serializer/Serializable.hpp
	src/SDL/SDLWindow.hpp
	src/SDL/SDLWindow.cpp
	src/Physics/GamePhysics.hpp
	src/Physics/GamePhysics.cpp
	src/Physics/GamePhysicsSettings.hpp
	src/Utils/Utils.hpp
	src/Utils/Utils.cpp
	src/Utils/ColliderChecks.hpp
	src/Utils/ColliderChecks.cpp
	src/Events/EventManager.hpp
	src/Events/EventManager.cpp
	src/Events/EventListener.hpp
	../vendor/tinyobjloader/tiny_obj_loader.h
	../vendor/stb/stb_image.h
	../vendor/ImGuizmo/ImGuizmo.h
	../vendor/ImGuizmo/ImGuizmo.cpp
	src/Input/InputEvent.cpp
	src/Input/InputEvent.hpp
	src/Input/InputListener.hpp
	src/Input/InputListener.cpp
	src/Input/InputFramework.hpp
	src/Input/InputFramework.cpp
	src/Audio/AudioManager.cpp
	src/Audio/AudioManager.hpp
	src/Multiplayer/Client/Client.hpp
	src/Multiplayer/Client/Client.cpp
	src/ScriptEngine/ScriptEngine.hpp
	src/ScriptEngine/ScriptEngine.cpp
	${SHADERS}
)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

source_group("Components" FILES
	src/Components/Component.hpp
	src/Components/PointLightComponent.hpp
	src/Components/PhysicsComponent.hpp
	src/Components/MeshComponent.hpp
	src/Components/MeshComponent.cpp
	src/Components/ColliderComponent.hpp
	src/Components/ColliderComponent.cpp
	src/Components/BoxColliderComponent.hpp
	src/Components/BoxColliderComponent.cpp
	src/Components/SphereColliderComponent.hpp
	src/Components/SphereColliderComponent.cpp
	src/Components/CameraComponent.hpp
	src/Components/CameraComponent.cpp
	src/Components/ComponentFactory.hpp
	src/Components/ComponentFactory.cpp
	src/Components/ControllableComponent.hpp
	src/Components/ControllableComponent.cpp
	src/Components/ScriptableComponent.hpp
	src/Components/ScriptableComponent.cpp
)

source_group("Renderer" FILES
	src/Renderer/Renderer.hpp
	src/Renderer/Renderer.cpp
	src/Renderer/GraphicsAPI.hpp
	src/Renderer/Buffer.hpp
	src/Renderer/Buffer.cpp
	src/Renderer/Shader.hpp
	src/Renderer/Shader.cpp
	src/Renderer/RendererSettings.hpp
	src/Renderer/Vertex.hpp
)

source_group("OpenGL" FILES
	src/Renderer/OpenGL/OpenGLGraphicsAPI.hpp
	src/Renderer/OpenGL/OpenGLGraphicsAPI.cpp
	src/Renderer/OpenGL/OpenGLBuffer.hpp
	src/Renderer/OpenGL/OpenGLBuffer.cpp
	src/Renderer/OpenGL/OpenGLShader.hpp
	src/Renderer/OpenGL/OpenGLShader.cpp
	src/Renderer/OpenGL/OpenGLTexture.hpp
	src/Renderer/OpenGL/OpenGLTexture.cpp
)

source_group("Events" FILES
	src/Events/EventManager.hpp
	src/Events/EventManager.cpp
	src/Events/EventListener.hpp
)

source_group("ScriptEngine" FILES
	src/ScriptEngine/ScriptEngine.hpp
	src/ScriptEngine/ScriptEngine.cpp
)


source_group("Serializer" FILES
	src/Serializer/GameSerializer.hpp
	src/Serializer/GameSerializer.cpp
	src/Serializer/Serializable.hpp
)

source_group("Utils" FILES
	src/Utils/Utils.hpp
	src/Utils/Utils.cpp
	src/Utils/ColliderChecks.hpp
	src/Utils/ColliderChecks.cpp
)


source_group("Physics" FILES
	src/Physics/GamePhysics.hpp
	src/Physics/GamePhysics.cpp
	src/Physics/GamePhysicsSettings.hpp
)

source_group("SDL" FILES
	src/SDL/SDLWindow.hpp
	src/SDL/SDLWindow.cpp
)


source_group("Core" FILES
	src/Core/Transform.hpp
	src/Core/Transform.cpp
	src/Core/GameObject.hpp
	src/Core/GameObject.cpp
	src/Core/Game.hpp
	src/Core/Game.cpp
	src/Core/Bootstrap.hpp
	src/Core/Bootstrap.cpp
	src/Core/Logger.hpp
	src/Core/Logger.cpp
	src/Core/Window.hpp
	src/Core/Window.cpp
	src/Core/Uuid.hpp
	src/Core/Uuid.cpp
	src/Core/Texture.hpp
	src/Core/Texture.cpp
	src/Core/Material.hpp
	src/Core/GameConfig.hpp
	src/Core/BoundingBox.hpp
	src/Core/BoundingBox.cpp
	src/Core/Debug.hpp
	src/Core/Debug.cpp
	src/Core/ResourceManager.hpp
	src/Core/ResourceManager.cpp
)

source_group("Vendor" FILES
	../vendor/tinyobjloader/tiny_obj_loader.h
	../vendor/stb/stb_image.h
	../vendor/ImGuizmo/ImGuizmo.h
	../vendor/ImGuizmo/ImGuizmo.cpp
)

source_group("Input" FILES
	src/Input/InputEvent.cpp
	src/Input/InputEvent.hpp
	src/Input/InputListener.hpp
	src/Input/InputListener.cpp
	src/Input/InputFramework.hpp
	src/Input/InputFramework.cpp
)

source_group("Audio" FILES
	src/Audio/AudioManager.cpp
	src/Audio/AudioManager.hpp
)

source_group("Multiplayer" FILES
	src/Multiplayer/Client/Client.hpp
	src/Multiplayer/Client/Client.cpp
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
	sol2::sol2
	winmm.lib
)

target_link_libraries(imguilib PRIVATE 
	imgui
	SDL3::SDL3
)
