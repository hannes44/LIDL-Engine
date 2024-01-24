#include "InputEvent.hpp"

namespace engine {

    // Constructor
    InputEvent::InputEvent(float x, float y, float button, std::string key, const std::string& classification)
        : x(x), y(y), button(button), key(key), classification(classification) {}

    // Getter functions
    float InputEvent::getX() const {
        return x;
    }

    float InputEvent::getY() const {
        return y;
    }

    float InputEvent::getButton() const {
        return button;
    }

    std::string InputEvent::getKey() const {
        return key;
    }

    const std::string& InputEvent::getClassification() const {
        return classification;
    }

    // Setter functions
    void InputEvent::setX(float x) {
        this->x = x;
    }

    void InputEvent::setY(float y) {
        this->y = y;
    }

    void InputEvent::setButton(float button) {
        this->button = button;
    }

    void InputEvent::setKey(std::string key) {
        this->key = key;
    }

    void InputEvent::setClassification(const std::string& classification) {
        this->classification = classification;
    }

}
