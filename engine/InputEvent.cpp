#include "InputEvent.hpp"

namespace engine {

    InputEvent::InputEvent(int x, int y, int button, int key, const std::string& classification)
        : x(x), y(y), button(button), key(key), classification(classification) {}

    int InputEvent::getX() const {
        return x;
    }

    void InputEvent::setX(int x) {
        this->x = x;
    }

    int InputEvent::getY() const {
        return y;
    }

    void InputEvent::setY(int y) {
        this->y = y;
    }

    int InputEvent::getButton() const {
        return button;
    }

    void InputEvent::setButton(int button) {
        this->button = button;
    }

    int InputEvent::getKey() const {
        return key;
    }

    void InputEvent::setKey(int key) {
        this->key = key;
    }

    const std::string& InputEvent::getClassification() const {
        return classification;
    }

    void InputEvent::setClassification(const std::string& classification) {
        this->classification = classification;
    }

}