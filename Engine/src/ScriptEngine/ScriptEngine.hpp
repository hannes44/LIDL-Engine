#pragma once
#include <iostream>
#include "Core/Game.hpp"

namespace engine
{
	class ScriptEngine
	{
	public:
		void start(Game* game);

		void addGameObject();

		Game* game;
	};

	class Cat
	{
	public:
		Cat() {};
		virtual ~Cat() {};
	public:
		void meow() { std::cout << "MEOWW" << std::endl; };
	};
}