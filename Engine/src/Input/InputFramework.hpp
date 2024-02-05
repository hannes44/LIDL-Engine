#pragma once
#include "InputSystem.hpp"
#include "InputEvent.hpp"
#include <SDL.h>
#include "Events/EventManager.hpp"
/*
    InputFramework class
	- This class is a singleton that inherits from InputSystem
	- The InputFramework class is responsible for reading input from the keyboard and mouse
    - It then dispatches the input events to all listeners though the InputSystem class
*/

namespace engine {

    class InputFramework : public InputSystem {
    public:
        // Non cloneable
        InputFramework(const InputFramework& other) = delete;

        // Non copyable
        void operator=(const InputFramework& other) = delete;

        static InputFramework& getInstance();

        void getInput() override;

        void initialize();

        void cleanup() override;

        bool isKeyPressed(const char* key);

        void handleContinousInput();

    private:
        // Private constructor to prevent external instantiation
        InputFramework();
        SDL_Event ev;

    };

}