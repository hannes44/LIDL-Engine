#pragma once
#include <string>

/*
    InputEvent class
    Stores information about an input event (e.g., mouse click, key press, etc.)
    Used by the InputSystem class to dispatch events to listeners (e.g., GameObjects)
*/


namespace engine {

    class InputEvent {
    public:
        InputEvent(float x, float y, float button, const std::string& key, const std::string& classification);

        float getX() const;
        float getY() const;
        float getButton() const;
        const std::string& getKey() const;
        const std::string& getClassification() const;

        void setX(float x);
        void setY(float y);
        void setButton(float button);
        void setKey(const std::string& key);
        void setClassification(const std::string& classification);

    private:
        float x;
        float y;
        float button;
        std::string key;
        std::string classification;
    };

}
