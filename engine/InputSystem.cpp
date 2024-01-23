#include "InputSystem.hpp"
#include "InputListener.hpp"
#include "InputEvent.hpp"
#include <list>

namespace engine
{
    std::list<InputListener> InputSystem::listeners;

    void InputSystem::addListener(const InputListener& listener) {
        for (const InputListener& l : listeners) {
            if (l.getInstance() == listener.getInstance()) {
                return;
            }
        }
        listeners.push_back(listener);
    }

    void InputSystem::removeListener(InputListener listener) {
        listeners.remove_if([&listener](const InputListener& l) {
            return l.getInstance() == listener.getInstance();
            });
    }

    void InputSystem::dispatchEvent(const InputEvent& e, std::string EventType) {
        for (const InputListener& l : listeners) {
            l.handleInput(e, EventType);
        }
    }

    void initialize() {
    }

	void getInput();
}