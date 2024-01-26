#include "SDLWindow.hpp"
#include "Logger.hpp"

namespace engine
{
	void SDLWindow::createWindow(int width, int height, std::string title)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			LOG_FATAL("SDL could not initialize! SDL_Error: {}", SDL_GetError());
		}

		atexit(SDL_Quit);
		SDL_GL_LoadLibrary(nullptr); // Default OpenGL is fine.

		// Request an OpenGL 4.1 context (should be core)
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

		// Multisampling
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


		window = SDL_CreateWindow(title.c_str(),
			width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		if (window == nullptr)
		{
			LOG_FATAL("{0}, Failed to create window", SDL_GetError());
			abort();
		}

		void* glContext = SDL_GL_CreateContext(window);

		SDL_GL_MakeCurrent(window, glContext);
	}
	void SDLWindow::cleanup()
	{
		//Destroy window
		closeWindow();

		//Quit SDL subsystems
		SDL_Quit();
	}
	void SDLWindow::getWindowSize(int* width, int* height)
	{
		SDL_GetWindowSize(window, width, height);
	}
	void SDLWindow::setWindowTitle(std::string title)
	{
		SDL_SetWindowTitle(window, title.c_str());
	}
	void SDLWindow::closeWindow()
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	void SDLWindow::newFrame()
	{
		SDL_GL_SwapWindow(window);
	}
}
