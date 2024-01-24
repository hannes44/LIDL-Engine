#pragma once
#include "GameObject.hpp"
#include "InputListener.hpp"

namespace engine {

    class Cube : public GameObject, public InputListener {
    public:
        Cube();

        // Implement the handleInput function
        void handleInput(const InputEvent& event, std::string EventType) const override;
    };

}