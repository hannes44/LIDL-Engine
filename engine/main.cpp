#include <iostream>
#include "TestGame.hpp"
#include "Logger.hpp"
#include <imgui.h>
#include <stdio.h>
#include <SDL3/SDL.h>

int main(int argc, char* argv[])
{
	engine::Logger::init();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	//ImGui::Text("Hello, world");
	ImGui::ShowDemoWindow();

	return 0;
}

