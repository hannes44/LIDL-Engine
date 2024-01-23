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
		virtual void initialize();

		static void addListener(const InputListener& listener);

		static void removeListener(InputListener listener);

		static void dispatchEvent(const InputEvent& e, std::string EventType);

		virtual void getInput() = 0;
	};
}