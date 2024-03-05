#pragma once

#include <string>
#include <SDL.h>

namespace engine
{
	enum WindowBackend
	{
		SDL3
	};

	class Window
	{
	public:
		Window() = default;

		// Non cloneable
		Window(Window& other) = delete;

		// Non copyable
		void operator=(const Window&) = delete;

		static Window& getInstance();

		virtual void createWindow(int width, int height, std::string title) = 0;

		virtual void cleanup() = 0;

		virtual void getWindowSize(int* width, int* height) = 0;
		virtual void setWindowTitle(std::string title) = 0;

		virtual void closeWindow() = 0;

		virtual void newFrame() = 0;

		virtual void setRelativeMouseMode(bool enabled) = 0;

		inline static Window* instance = nullptr;
	private:

		inline static WindowBackend backend = WindowBackend::SDL3;
	};
} // namespace engine