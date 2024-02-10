#pragma once
#include "InputEvent.hpp"
#include <string>

/*
	InputListener class
	- This class is an abstract class that can be inherited from to create a listener for input events
	- The handleInput function is called by the InputSystem when an input event is dispatched
	- The handleInput function can be overridden in a subclass to handle input events
	- Any object interested in handling input events should inherit from this class
	  and subscribe to the InputSystem by calling InputSystem::addListener(this)

	 handleInput function example:
	 void handleInput(const InputEvent& event, const std::string& EventType) override {
		 if (EventType == "KEY_DOWN") {
			 if (event.key == Key::A) {
				 // Do something ex. set acceleration to 1
			 }
		 } else if (EventType == "KEY_UP") {
			 if (event.button == Key::A) {
				 // Do something ex. set acceleration to 0
			 }
		 }
	 }
*/

namespace engine {

	class InputListener {
	public:
		InputListener();

		virtual void handleInput(const InputEvent& event);

		std::string getInstance() const;

	private:
		static int instanceCount;
		int instance;

	};

}
