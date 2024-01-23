#include <iostream>
#include "TestGame.hpp"
#include "Logger.hpp"
#include <GL/glew.h>
#include "Window.hpp"

int main(int argc, char* argv[])
{
	engine::Logger::init();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	engine::Window::getInstance().createWindow(800, 600, "Test Window");

	glewInit();


	
	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
		}
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(engine::Window::getInstance().window);
	}

	return 0;
}

