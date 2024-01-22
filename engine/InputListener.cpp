#include "InputListener.hpp"
#include "InputEvent.hpp"
#include <iostream>

namespace engine
{
    // InputListener implementation
    InputListener::InputListener(std::string instance) : instance(instance) {}

    std::string InputListener::getInstance() const {
        return instance;
    }

    virtual void InputListener::handleInput(const InputEvent& event) const {
        // Add your input handling logic here
        std::cout << "InputListener " << instance << " handles input event with classification: "
            << event.getClassification() << std::endl;
        // You can access other information from the event, e.g., event.getX(), event.getY(), etc.
    }
} 