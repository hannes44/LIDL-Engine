#include "Cube.hpp"
#include <iostream>

namespace engine {

    Cube::Cube() : InputListener() {
        // Additional initialization logic can be added here
    }

    void Cube::handleInput(const InputEvent& event, std::string EventType) const {
        // Handle input for the Cube
        Bootstrap::getInstance().getDisplay().drawText(event.getKey(), 0, 2, Color::RED);
        // You can add more specific input handling logic for the Cube
    }

}