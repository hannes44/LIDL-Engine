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

            if (ev.type == SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN)
            {
                SDL.SDL_MouseButtonEvent butt;

                butt = ev.button;

                ie.Button = (int)butt.button;
                ie.X = butt.x;
                ie.Y = butt.y;

                informListeners(ie, "MouseDown");
            }

            if (ev.type == SDL.SDL_EventType.SDL_MOUSEBUTTONUP)
            {
                SDL.SDL_MouseButtonEvent butt;

                butt = ev.button;

                ie.Button = (int)butt.button;
                ie.X = butt.x;
                ie.Y = butt.y;

                informListeners(ie, "MouseUp");
            }

            if (ev.type == SDL.SDL_EventType.SDL_MOUSEWHEEL)
            {
                SDL.SDL_MouseWheelEvent wh;

                wh = ev.wheel;

                ie.X = (int)wh.direction * wh.x;
                ie.Y = (int)wh.direction * wh.y;

                informListeners(ie, "MouseWheel");
            }


            if (ev.type == SDL.SDL_EventType.SDL_KEYDOWN)
            {
                ie.Key = (int)ev.key.keysym.scancode;
                Debug.getInstance().log("Keydown: " + ie.Key);
                informListeners(ie, "KeyDown");
            }

            if (ev.type == SDL.SDL_EventType.SDL_KEYUP)
            {
                ie.Key = (int)ev.key.keysym.scancode;
                informListeners(ie, "KeyUp");
            }

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