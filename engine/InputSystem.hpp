#pragma once
#include "InputListener.hpp"
#include <list>

namespace engine
{
	class InputSystem
	{
	private:
		static std::list<InputListener> listeners;

	public:
		void initialize();

		static void addListener(InputListener listener);

		static void removeListener(InputListener listener);

		static void dispatchEvent(const InputEvent& e);

		virtual void getInput() = 0;
	};
}