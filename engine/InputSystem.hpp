#pragma once
#include "InputListener.hpp"
#include <list>

/*
    InputSystem class
    - This class is an abstract class that defines the interface between the input framework and the game objects
    - The InputSystem class is responsible for dispatching input events to all listeners
    - The InputSystem class is also responsible for adding and removing listeners (subscribers)
*/

namespace engine {

    class InputSystem {
    public:
        // Function to initialize the input system
        static void initialize();

        // Function to add a listener
        static void addListener(InputListener* listener);

        // Function to remove a listener
        static void removeListener(InputListener* listener);

        // Function to dispatch an event to all listeners
        static void dispatchEvent(const InputEvent& e, const std::string& EventType);

        // Function to get input (to be implemented by derived classes)
        virtual void getInput() = 0;

        // Function to clean up resources
        virtual void cleanup() = 0;

    private:
        // Data members
        static std::list<InputListener*> listeners;

    };

}
