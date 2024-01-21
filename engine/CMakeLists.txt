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
)
target_link_libraries(Engine PRIVATE vendor)