#pragma once
#include "InputEvent.hpp"
#include <SDL.h>
#include "Events/EventManager.hpp"
#include "InputListener.hpp"
#include <list>
#include <string>
/*
	InputFramework class
	- This class is a singleton that inherits from InputSystem
	- The InputFramework class is responsible for reading input from the keyboard and mouse
	- It then dispatches the input events to all listeners though the InputSystem class
*/

namespace engine {

	class InputFramework {
	public:
		// Non cloneable
		InputFramework(const InputFramework& other) = delete;

		// Non copyable
		void operator=(const InputFramework& other) = delete;

		static InputFramework& getInstance();

		void getInput();

		void cleanup();

		bool isKeyPressed(const char* key);

		void handleContinousInput();

		void addListener(InputListener* listener);

		void removeListener(InputListener* listener);

		void dispatchEvent(const InputEvent& e);

		inline static InputFramework* instance;
	private:
		// Private constructor to prevent external instantiation
		InputFramework();
		SDL_Event ev;
		std::list<InputListener*> listeners{};
	};

}