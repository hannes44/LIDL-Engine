#include "InputFramework.hpp"
#include "InputEvent.hpp"
#include <SDL.h>
#include "Bootstrap.hpp"

namespace engine {

    // Constructor
    InputFramework::InputFramework() : tick(0), timeInterval(1.0 / 60.0) {}

    // Override getInput from InputSystem
    void InputFramework::getInput() {
        SDL_Event ev;
        InputEvent ie(0, 0, 0, 0, "");  // Initialize with default values

        tick += Bootstrap::getDeltaTime();

        if (tick < timeInterval) {
            return;
        }

        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_MOUSEMOTION) {
                SDL_MouseMotionEvent mot = ev.motion;
                ie.setX(mot.x);
                ie.setY(mot.y);
                dispatchEvent(ie, "MouseMotion");
            }

            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                SDL_MouseButtonEvent butt = ev.button;
                ie.setButton(static_cast<int>(butt.button));
                ie.setX(butt.x);
                ie.setY(butt.y);
                dispatchEvent(ie, "MouseDown");
            }

            if (ev.type == SDL_MOUSEBUTTONUP) {
                SDL_MouseButtonEvent butt = ev.button;
                ie.setButton(static_cast<int>(butt.button));
                ie.setX(butt.x);
                ie.setY(butt.y);
                dispatchEvent(ie, "MouseUp");
            }

            if (ev.type == SDL_MOUSEWHEEL) {
                SDL_MouseWheelEvent wh = ev.wheel;
                ie.setX(static_cast<int>(wh.direction) * wh.x);
                ie.setY(static_cast<int>(wh.direction) * wh.y);
                dispatchEvent(ie, "MouseWheel");
            }

            if (ev.type == SDL_KEYDOWN) {
                ie.setKey(static_cast<int>(ev.key.keysym.scancode));
                //Debug::getInstance().log("Keydown: " + std::to_string(ie.getKey()));
                dispatchEvent(ie, "KeyDown");
            }

            if (ev.type == SDL_KEYUP) {
                ie.setKey(static_cast<int>(ev.key.keysym.scancode));
                dispatchEvent(ie, "KeyUp");
            }
        }

        tick -= timeInterval;
    }

    // Override initialize from InputSystem
    void InputFramework::initialize() {
        tick = 0;
        timeInterval = 1.0 / 60.0;
    }

}