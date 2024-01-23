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
)

source_group("OpenGL" FILES
	OpenGLGraphicsAPI.hpp
)

target_sources(Engine PRIVATE 
	main.cpp
	Transform.hpp
	Transform.cpp
	GameObject.hpp
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
	../vendor/tinyobjloader/tiny_obj_loader.h
)
target_link_libraries(Engine PRIVATE vendor)