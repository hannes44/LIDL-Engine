#pragma once
#include "Display.hpp"
#include "DisplayTerminal.hpp"

namespace engine
{
	class Bootstrap
	{
	private:
		inline static Display* display = new DisplayTerminal();
	protected:
		Bootstrap() {};

		inline static Bootstrap* instance;
	public:
		// Non cloneable
		Bootstrap(Bootstrap& other) = delete;

		// Non copyable
		void operator=(const Bootstrap&) = delete;

		static Bootstrap& getInstance();

		static Display* getDisplay();
	};
}