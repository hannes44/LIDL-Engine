add_executable(Engine)
target_sources(Engine PRIVATE 
	main.cpp
	transform.hpp
	transform.cpp
	game_object.hpp
	game_object_manager.hpp
	game_object_manager.cpp
	component.hpp
	point_light_component.hpp
	physics_component.hpp
	Game.hpp
	Game.cpp
	TestGame.hpp
	TestGame.cpp
	Display.hpp
	DisplayTerminal.hpp
	DisplayTerminal.cpp
	Bootstrap.hpp
	Bootstrap.cpp
)
target_link_libraries(Engine PRIVATE vendor)