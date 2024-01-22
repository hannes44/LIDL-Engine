#include <iostream>
#include "transform.hpp"
#include "TestGame.hpp"
#include "logger.hpp"
#include "uuid_v4.h"

int main(int argc, char* argv[])
{
	engine::Logger::init();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
	UUIDv4::UUID uuid = uuidGenerator.getUUID();
	std::string s = uuid.str();

	std::cout << s;

	return 0;
}

