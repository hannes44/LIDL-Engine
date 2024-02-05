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
        static void initialize();

        static void addListener(InputListener* listener);

        static void removeListener(InputListener* listener);

        static void dispatchEvent(const InputEvent& e);

        virtual void getInput() = 0;

        virtual void cleanup() = 0;

    private:
        static std::list<InputListener*> listeners;

    };

}
