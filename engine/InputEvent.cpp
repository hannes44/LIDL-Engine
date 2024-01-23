#include "InputEvent.hpp"

namespace engine {

    InputEvent::InputEvent(float x, float y, float button, std::string key, const std::string& classification)
        : x(x), y(y), button(button), key(key), classification(classification) {}

    float InputEvent::getX() const {
        return x;
    }

    void InputEvent::setX(float x) {
        this->x = x;
    }

    float InputEvent::getY() const {
        return y;
    }

    void InputEvent::setY(float y) {
        this->y = y;
    }

    float InputEvent::getButton() const {
        return button;
    }

    void InputEvent::setButton(float button) {
        this->button = button;
    }

    std::string InputEvent::getKey() const {
        return key;
    }

    void InputEvent::setKey(std::string key) {
        this->key = key;
    }

    const std::string& InputEvent::getClassification() const {
        return classification;
    }

    void InputEvent::setClassification(const std::string& classification) {
        this->classification = classification;
    }

}