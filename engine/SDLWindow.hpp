#pragma once

#include "Window.hpp"

namespace engine
{
	class SDLWindow : public Window
	{
	public:
		SDLWindow() {};

		void createWindow(int width, int height, std::string title) override;

		void cleanup() override;

		void getWindowSize(int* width, int* height) override;

		void setWindowTitle(std::string title) override;

		void closeWindow() override;

		void newFrame() override;
	private:
		SDL_Window* window = nullptr;
	};
}