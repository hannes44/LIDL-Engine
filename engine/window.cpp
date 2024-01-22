#include "Window.hpp"

namespace engine
{
	Window& Window::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Window();
		}
		return *instance;
	}

	void Window::createWindow(int width, int height, const std::string title)
	{
		window = SDL_CreateWindow(title.c_str(),
			width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}

	void Window::cleanup()
	{
		//Destroy window
		closeWindow();

		//Quit SDL subsystems
		SDL_Quit();
	}

	void Window::getWindowSize(int* width, int* height)
	{
		SDL_GetWindowSize(window, width, height);
	}

	void Window::setWindowTitle(const std::string title)
	{
		SDL_SetWindowTitle(window, title.c_str());
	}

	void Window::closeWindow()
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	void Window::newFrame()
	{
		SDL_Event test_event;
		while (SDL_PollEvent(&test_event)) 
		{
			if (test_event.type == SDL_EVENT_QUIT)
			{
				cleanup();
			}
		}
	}

}
