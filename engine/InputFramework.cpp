#include "InputFramework.hpp"
#include <SDL.h>

namespace engine
{
    // Constructor
    InputFramework::InputFramework() : tick(0), timeInterval(1.0 / 60.0) {}

    // Override getInput from InputSystem
    void InputFramework::getInput() {
        SDL_Event ev;
        int res;
        InputEvent ie;

        tick += Bootstrap::getDeltaTime();

        if (tick < timeInterval) {
            return;
        }

        while (tick >= timeInterval) {
            res = SDL_PollEvent(&ev);

            if (res != 1) {
                return;
            }

            ie = InputEvent();  // Assuming InputEvent has a default constructor

            if (ev.type == SDL_MOUSEMOTION) {
                ie.setX(ev.motion.x);
                ie.setY(ev.motion.y);
                dispatchEvent(ie, "MouseMotion");
            }

            // Handle other SDL events...

            tick -= timeInterval;
        }
    }

    // Override initialize from InputSystem
    void InputFramework::initialize() {
        tick = 0;
        timeInterval = 1.0 / 60.0;
    }

}

}