#pragma once
#include "InputListener.hpp"
#include <list>

namespace engine {

    class InputSystem {
    private:
        static std::list<InputListener*> listeners;

    public:
        // Function to initialize the input system
        static void initialize();

        // Function to add a listener
        static void addListener(InputListener* listener);

        // Function to remove a listener
        static void removeListener(InputListener* listener);

        // Function to dispatch an event to all listeners
        static void dispatchEvent(const InputEvent& e, std::string EventType);

        // Function to get input (to be implemented by derived classes)
        virtual void getInput() = 0;

        // Function to clean up resources (to be implemented by derived classes)
        virtual void cleanup() = 0;
    };

}
