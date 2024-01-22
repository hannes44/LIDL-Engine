#include "InputEvent.hpp"
#include <string>

namespace engine
{
    InputEvent::InputEvent(int x, int y, int button, int key, const std::string& classification)
        : x(x), y(y), button(button), key(key), classification(classification) {}

    int InputEvent::getX() const {
        return x;
    }

    int InputEvent::getY() const {
        return y;
    }

    int InputEvent::getButton() const {
        return button;
    }

    int InputEvent::getKey() const {
        return key;
    }

    const std::string& InputEvent::getClassification() const {
        return classification;
    }

}