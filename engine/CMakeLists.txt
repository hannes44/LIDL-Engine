add_executable(Engine)

source_group("Components" FILES
	Component.hpp
	PointLightComponent.hpp
	PhysicsComponent.hpp
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
)
target_link_libraries(Engine PRIVATE vendor)