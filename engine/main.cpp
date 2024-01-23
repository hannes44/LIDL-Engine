#include <iostream>
#include "TestGame.hpp"
#include "Logger.hpp"

int main(int argc, char* argv[])
{
	engine::Logger::init();

	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();

	return 0;
}

