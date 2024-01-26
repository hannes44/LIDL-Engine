#pragma once
#include "Display.hpp"
#include "DisplayTerminal.hpp"

namespace engine
{
	class Bootstrap
	{
	public:
		// Non cloneable
		Bootstrap(Bootstrap& other) = delete;

		// Non copyable
		void operator=(const Bootstrap&) = delete;

		static Bootstrap& getInstance();

		Display& getDisplay();

		static double getDeltaTime();

    
	protected:
		Bootstrap() {};

	private:
		Display* display = new DisplayTerminal();
		inline static Bootstrap* instance = nullptr;
	};
}