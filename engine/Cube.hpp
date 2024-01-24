#pragma once
#include "GameObject.hpp"
#include "InputListener.hpp"
#include "Bootstrap.hpp"

namespace engine {

    class Cube : public GameObject, public InputListener {
    public:
        Cube();

        // Implement the handleInput function
        void handleInput(const InputEvent& event, const std::string& EventType) const override;
    };

}