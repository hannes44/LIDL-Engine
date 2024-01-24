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
*/

namespace engine {

    class InputListener {
    public:
        // Constructor
        InputListener();

        // Function to handle input events
        virtual void handleInput(const InputEvent& event, const std::string& EventType) const;

        // Getter for instance
        std::string getInstance() const;

    private:
        // Data members
        static int instanceCount;
        int instance;

    };

}
