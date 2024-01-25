#include "InputFramework.hpp"
#include "Bootstrap.hpp"
#include <SDL.h>

/*
    This is the implementation of the InputFramework class
    Please refer to the header for more information.
*/

namespace engine {

    InputFramework::InputFramework() : tick(0), timeInterval(1.0 / 60.0) {}

    InputFramework& InputFramework::getInstance() {
        static InputFramework instance;
        return instance;
    }

    // Read input from the SDL window
    void InputFramework::getInput() {
        SDL_Event ev;
        InputEvent ie(0, 0, 0, "", "");  // Initialize with default values

        tick += Bootstrap::getInstance().getDeltaTime();

        if (tick < timeInterval) {
            return;
        }

        if (SDL_PollEvent(&ev) != 0) {
            // Check if there are any events to process
            while (SDL_PollEvent(&ev)) {
                // Handle different types of input events and dispatch them
                // to registered listeners using InputSystem::dispatchEvent.

                if (ev.type == SDL_EVENT_MOUSE_MOTION) {
                    SDL_MouseMotionEvent mot = ev.motion;
                    ie.setX(mot.x);
                    ie.setY(mot.y);
                    dispatchEvent(ie, "MouseMotion");
                }
                else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    SDL_MouseButtonEvent but = ev.button;
                    ie.setX(but.x);
                    ie.setY(but.y);
                    ie.setButton(but.button);
                    dispatchEvent(ie, "MouseButtonDown");
                }
                else if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    SDL_MouseButtonEvent but = ev.button;
                    ie.setX(but.x);
                    ie.setY(but.y);
                    ie.setButton(but.button);
                    dispatchEvent(ie, "MouseButtonUp");
                }
                else if (ev.type == SDL_EVENT_KEY_DOWN) {
                    SDL_KeyboardEvent key = ev.key;
                    ie.setKey(SDL_GetKeyName(key.keysym.sym));
                    dispatchEvent(ie, "KeyDown");
                }
                else if (ev.type == SDL_EVENT_KEY_UP) {
                    SDL_KeyboardEvent key = ev.key;
                    ie.setKey(SDL_GetKeyName(key.keysym.sym));
                    dispatchEvent(ie, "KeyUp");
                }
                else if (ev.type == SDL_EVENT_QUIT) {
                    dispatchEvent(ie, "Quit");
                }
            }
        }
        else {
            // Error retrieving event
            std::cerr << "SDL_PollEvent error: " << SDL_GetError() << std::endl;
        }

        tick -= timeInterval;
    }

    // Initialize the input framework
    void InputFramework::initialize() {
        tick = 0;
        timeInterval = 1.0 / 60.0;
        // Additional initialization logic can be added here
        InputSystem::initialize();
    }

    // Clean up resources
    void InputFramework::cleanup() {
        InputSystem::cleanup();
        // Implement cleanup logic if needed
    }

}