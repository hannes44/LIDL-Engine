#pragma once

#include <string>
#include <SDL.h>

namespace engine
{
	class Window
	{
	public:
		// Non cloneable
		Window(Window& other) = delete;

		// Non copyable
		void operator=(const Window&) = delete;

		Window(Window const&) = delete;

		static Window& getInstance();

		void createWindow(int width, int height, std::string title);

		void cleanup();

		void getWindowSize(int* width, int* height);
		void setWindowTitle(std::string title);

		void closeWindow();

		void newFrame();

	private:
		inline static Window* instance = nullptr;
		Window() {};

		SDL_Window* window = nullptr;
	};
} // namespace engine