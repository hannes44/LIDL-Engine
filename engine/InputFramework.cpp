#include "InputFramework.hpp"
#include "Bootstrap.hpp"
#include <SDL.h>

namespace engine {

    InputFramework::InputFramework() : tick(0), timeInterval(1.0 / 60.0) {}

    InputFramework& InputFramework::getInstance() {
        static InputFramework instance;
        return instance;
    }

    void InputFramework::getInput() {
        SDL_Event ev;
        InputEvent ie(0, 0, 0, "", "");  // Initialize with default values

        tick += Bootstrap::getDeltaTime();

        if (tick < timeInterval) {
            return;
        }

        while (SDL_PollEvent(&ev)) {
            // Handle different types of input events and dispatch them
            // to registered listeners using InputSystem::dispatchEvent.

            // Example:
            // if (ev.type == SDL_MOUSEMOTION) {
            //     ie.setX(ev.motion.x);
            //     ie.setY(ev.motion.y);
            //     dispatchEvent(ie, "MouseMotion");
            // }

            // Add similar blocks for other types of input events.
        }

        tick -= timeInterval;
    }

    void InputFramework::initialize() {
        tick = 0;
        timeInterval = 1.0 / 60.0;
        // Additional initialization logic can be added here
    }

}