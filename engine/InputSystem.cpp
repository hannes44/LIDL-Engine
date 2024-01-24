#include "InputSystem.hpp"

namespace engine {

    std::list<InputListener*> InputSystem::listeners;

    void InputSystem::initialize() {
        // Additional initialization logic can be added here
        listeners.clear();
    }

    void InputSystem::addListener(InputListener* listener) {
        listeners.push_back(listener);
    }

    void InputSystem::removeListener(InputListener* listener) {
        listeners.remove(listener);
    }

    void InputSystem::dispatchEvent(const InputEvent& e, std::string EventType) {
        for (auto listener : listeners) {
            listener->handleInput(e, EventType);
        }
    }

}