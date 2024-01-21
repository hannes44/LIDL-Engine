#include <iostream>
#include "transform.hpp"
#include "TestGame.hpp"

int main(int argc, char* argv[])
{
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	
	return 0;
}

