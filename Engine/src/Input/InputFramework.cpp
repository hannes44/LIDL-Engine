#include "InputFramework.hpp"
#include "Core/Bootstrap.hpp"
#include "Core/Logger.hpp"
#include <SDL.h>
#include <imgui_impl_sdl3.h>

/*
    This is the implementation of the InputFramework class
    Please refer to the header for more information.
*/

namespace engine {

    InputFramework::InputFramework() {
    }

    InputFramework& InputFramework::getInstance() {
        if (instance == nullptr) {
			instance = new InputFramework();
		}
        return *instance;
    }

    bool InputFramework::isKeyPressed(const char* key) {
        SDL_Keycode keycode = SDL_GetKeyFromName(key);
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        return keystates[SDL_GetScancodeFromKey(keycode)];
    }

    // Handle continuous input for keys
    void InputFramework::handleContinousInput() {
        // Initialize with default values
        InputEvent ie(0, 0, 0, Key::LAST, InputEventType::NULL_EVENT);
        
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        for (int keyInt = (int)Key::A; (int)keyInt != (int)Key::LAST; keyInt++) {
            Key key = static_cast<Key>(keyInt);
            if (keystates[keyInt]) {
				ie.setKey(key);
                ie.setEventType(InputEventType::KeyHold);
				dispatchEvent(ie);
			}
        }
    }

    // Read input from the SDL window
    void InputFramework::getInput() {
        // Initialize with default values
        InputEvent ie(0, 0, 0, Key::LAST, InputEventType::NULL_EVENT);

        SDL_PumpEvents();
        handleContinousInput();

        // Poll for events and dispatch them
        while (SDL_PollEvent(&ev)) {

            ImGui_ImplSDL3_ProcessEvent(&ev);

            // If the user closes the window
            if (ev.type == SDL_EVENT_QUIT) {
				EventManager::getInstance().notify(EventType::QuitProgram);
			}

            if (ev.type == SDL_EVENT_MOUSE_MOTION) {
                SDL_MouseMotionEvent mot = ev.motion;
                SDL_MouseButtonEvent but = ev.button;
                ie.setButton(but.button);
                ie.setX(mot.xrel);
                ie.setY(mot.yrel);
                ie.setEventType(InputEventType::MouseMotion);
                dispatchEvent(ie);
            }
            if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_MouseButtonEvent but = ev.button;
                ie.setX(but.x);
                ie.setY(but.y);
                ie.setButton(but.button);
                ie.setEventType(InputEventType::MouseButtonDown);
                dispatchEvent(ie);
            }
            if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                SDL_MouseButtonEvent but = ev.button;
                ie.setX(but.x);
                ie.setY(but.y);
                ie.setButton(but.button);
                ie.setEventType(InputEventType::MouseButtonUp);
                dispatchEvent(ie);
            }
            if (ev.type == SDL_EVENT_KEY_DOWN && ev.key.repeat == 0) {
                SDL_KeyboardEvent key = ev.key;
                ie.setKey((Key)key.keysym.scancode);
                ie.setEventType(InputEventType::KeyDown);
                dispatchEvent(ie);
            }
            if (ev.type == SDL_EVENT_KEY_UP) {
                SDL_KeyboardEvent key = ev.key;
                ie.setKey((Key)key.keysym.scancode);
                ie.setEventType(InputEventType::KeyUp);
                dispatchEvent(ie);
            }
        }
    }

    // Clean up resources
    void InputFramework::cleanup() {
        listeners.clear();
    }

    // Function to add a listener (subscriber)
    void InputFramework::addListener(InputListener* listener) {
        listeners.push_back(listener);
    }

    // Function to remove a listener (subscriber)
    void InputFramework::removeListener(InputListener* listener) {
        listeners.remove(listener);
    }

    // Function to dispatch captured event to all listeners
    void InputFramework::dispatchEvent(const InputEvent& e) {
        for (auto* listener : listeners) {
            if (listener) {
                listener->handleInput(e);
            }
        }
    }

}