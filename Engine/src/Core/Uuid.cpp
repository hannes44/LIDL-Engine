#include "Uuid.hpp"
#include <random>
#include <functional>
#include <string>
#include <iostream>

namespace engine
{
	// Temporary string implementation
	// TODO: Implement a proper UUID generator
	UUID::UUID()
	{
		int len = 32;
		const char alphanum[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		std::string tmp_s;
		tmp_s.reserve(len);

		std::random_device rd;  // a seed source for the random number engine
		std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

		for (int i = 0; i < len; ++i) {
			tmp_s += alphanum[std::uniform_int_distribution<int>(0, 100)(gen) % (sizeof(alphanum) - 1)];
		}
		id = tmp_s;
	}
}
