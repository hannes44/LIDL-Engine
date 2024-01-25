#include "InputEvent.hpp"

/*
	This is the functionallity for the InputEvent class.
    It consists of a constructor, getters and setters.
*/

namespace engine {

    InputEvent::InputEvent(float x, float y, float button, const std::string& key)
        : x(x), y(y), button(button), key(key) {}

    float InputEvent::getX() const {
        return x;
    }

    float InputEvent::getY() const {
        return y;
    }

    float InputEvent::getButton() const {
        return button;
    }

    const std::string& InputEvent::getKey() const {
        return key;
    }

    void InputEvent::setX(float x) {
        this->x = x;
    }

    void InputEvent::setY(float y) {
        this->y = y;
    }

    void InputEvent::setButton(float button) {
        this->button = button;
    }

    void InputEvent::setKey(const std::string& key) {
        this->key = key;
    }

}
