#include <iostream>
#include "transform.hpp"
#include "TestGame.hpp"
#include "logger.hpp"
#include "GL/glew.h"

int main(int argc, char* argv[])
{
	engine::Logger::init();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	glewInit();

	return 0;
}

