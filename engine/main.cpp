#include <iostream>
#include "transform.hpp"
#include "TestGame.hpp"
#include "logger.hpp"
#include "uuid.hpp"
#include "MeshComponent.hpp"


int main(int argc, char* argv[])
{
	engine::Logger::init();

	engine::Uuid test;
	std::cout << test.getId();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	return 0;
}

