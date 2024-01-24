add_executable(Engine)

source_group("Components" FILES
	Component.hpp
	PointLightComponent.hpp
	PhysicsComponent.hpp
	MeshComponent.hpp
	MeshComponent.cpp
)

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
	Vertex.cpp
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
	../vendor/tinyobjloader/tiny_obj_loader.h
)
target_link_libraries(Engine PRIVATE vendor)