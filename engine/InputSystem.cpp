#include "InputSystem.hpp"

/*
    This file contains the implementation of the InputSystem class
    Please refer to the header for more information.
*/

namespace engine {

    std::list<InputListener*> InputSystem::listeners;

    // Function to initialize the input system
    void InputSystem::initialize() {
        listeners.clear();
        // Additional initialization logic can be added here
    }

    // Function to add a listener (subscriber)
    void InputSystem::addListener(InputListener* listener) {
        listeners.push_back(listener);
    }

    // Function to remove a listener (subscriber)
    void InputSystem::removeListener(InputListener* listener) {
        listeners.remove(listener);
    }

    // Function to dispatch captured event to all listeners
    void InputSystem::dispatchEvent(const InputEvent& e, const std::string& EventType) {
        for (auto *listener : listeners) {
            if (listener) {
                listener->handleInput(e, EventType);
            }
        }
    }

    // Function to clean up resources
    void InputSystem::cleanup() {
		listeners.clear();
	}

}