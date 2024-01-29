#pragma once
#include <string>
#include <SDL.h>

/*
    InputEvent class
    Stores information about an input event (e.g., mouse click, key press, etc.)
    Used by the InputSystem class to dispatch events to listeners (e.g., GameObjects)
*/


namespace engine {
    enum class Key
    {
        // Letter keys
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,
        // Number keys
        NUM_1 = SDL_SCANCODE_1,
        NUM_2 = SDL_SCANCODE_2,
        NUM_3 = SDL_SCANCODE_3,
        NUM_4 = SDL_SCANCODE_4,
        NUM_5 = SDL_SCANCODE_5,
        NUM_6 = SDL_SCANCODE_6,
        NUM_7 = SDL_SCANCODE_7,
        NUM_8 = SDL_SCANCODE_8,
        NUM_9 = SDL_SCANCODE_9,
        NUM_0 = SDL_SCANCODE_0,
        // Function keys
        RETURN = SDL_SCANCODE_RETURN,
        ESCAPE = SDL_SCANCODE_ESCAPE,
        BACKSPACE = SDL_SCANCODE_BACKSPACE,
        TAB = SDL_SCANCODE_TAB,
        SPACE = SDL_SCANCODE_SPACE,
        MINUS = SDL_SCANCODE_MINUS,
        EQUALS = SDL_SCANCODE_EQUALS,
        LSHIFT = SDL_SCANCODE_LSHIFT,
        LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET,
        RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
        BACKSLASH = SDL_SCANCODE_BACKSLASH,
        NONUSHASH = SDL_SCANCODE_NONUSHASH,
        SEMICOLON = SDL_SCANCODE_SEMICOLON,
        APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
        GRAVE = SDL_SCANCODE_GRAVE,
        COMMA = SDL_SCANCODE_COMMA,
        PERIOD = SDL_SCANCODE_PERIOD,
        SLASH = SDL_SCANCODE_SLASH,
        CAPSLOCK = SDL_SCANCODE_CAPSLOCK,
        // F keys
        F1 = SDL_SCANCODE_F1,
        F2 = SDL_SCANCODE_F2,
        F3 = SDL_SCANCODE_F3,
        F4 = SDL_SCANCODE_F4,
        F5 = SDL_SCANCODE_F5,
        F6 = SDL_SCANCODE_F6,
        F7 = SDL_SCANCODE_F7,
        F8 = SDL_SCANCODE_F8,
        F9 = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10,
        F11 = SDL_SCANCODE_F11,
        F12 = SDL_SCANCODE_F12,
        // Home/End keys
        PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
        SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK,
        PAUSE = SDL_SCANCODE_PAUSE,
        INSERT = SDL_SCANCODE_INSERT,
        HOME = SDL_SCANCODE_HOME,
        PAGEUP = SDL_SCANCODE_PAGEUP,
        DELETE = SDL_SCANCODE_DELETE,
        END = SDL_SCANCODE_END,
        LAST = 255,
    };
    class InputEvent {
    public:
        InputEvent(float x, float y, float button, const Key key);
        void clear();

        float getX() const;
        float getY() const;
        float getButton() const;
        const Key getKey() const;

        void setX(float x);
        void setY(float y);
        void setButton(float button);
        void setKey(const Key key);

    private:
        float x;
        float y;
        float button;
        Key key;
    };

}
