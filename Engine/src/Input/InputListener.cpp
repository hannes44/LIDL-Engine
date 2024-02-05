#include "InputListener.hpp"
#include <iostream>

/*
    This is the implementation of the InputListener class.
    Please refer to the header for more information.
*/

namespace engine {

    int InputListener::instanceCount = 0;

    InputListener::InputListener() : instance(instanceCount++) {}

    std::string InputListener::getInstance() const {
        return "Listener_" + std::to_string(instance);
    }

    // Function to handle input events (default implementation) || override this function in a subclass
    void InputListener::handleInput(const InputEvent& event, const std::string& EventType) {
        // Add your input handling logic here || or override this function in a subclass
        // You can access other information from the event, e.g., event.getX(), event.getY(), etc.
    }

}
