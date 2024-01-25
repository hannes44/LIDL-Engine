#include "Window.hpp"
#include "Logger.hpp"
#include "SDLWindow.hpp"

namespace engine
{
	Window& Window::getInstance()
	{
		if (instance == nullptr)
		{
			switch (backend)
			{
			case engine::SDL3:
				instance = new SDLWindow();
				break;
			default:
				LOG_FATAL("Window backend not supported");
				break;
			}
		}
		return *instance;
	}
}
