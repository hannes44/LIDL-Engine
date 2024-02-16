#pragma once
#include "InputEvent.hpp"
#include <SDL.h>
#include "Events/EventManager.hpp"
#include "InputListener.hpp"
#include "ActionMap.hpp"
#include <list>
#include <string>
/*
	InputFramework class
	- This class is a singleton that also inherits some functionality from the ActionMap class
	- The InputFramework class is responsible for reading input from the keyboard and mouse
	- It then dispatches input events to all listeners with the appropriate data
*/

namespace engine {

	class InputFramework : public ActionMap {
	public:
		// Non cloneable
		InputFramework(const InputFramework& other) = delete;

		// Non copyable
		void operator=(const InputFramework& other) = delete;

		static InputFramework& getInstance();

		void getInput();

		void cleanup();

		bool isKeyPressed(const char* key);

		bool allKeysPressed(const std::string action, std::list<Key> keysPressed);

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