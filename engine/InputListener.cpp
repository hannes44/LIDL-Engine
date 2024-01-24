#include "InputListener.hpp"
#include <iostream>

namespace engine {

    int InputListener::instanceCount = 0;

    InputListener::InputListener() : instance(instanceCount++) {}

    std::string InputListener::getInstance() const {
        return "Listener_" + std::to_string(instance);
    }

    void InputListener::handleInput(const InputEvent& event, std::string EventType) const {
        // Add your input handling logic here || or override this function in a subclass
        // 
        // You can access other information from the event, e.g., event.getX(), event.getY(), etc.
    }

}
