#pragma once

#include "Component.hpp"
#include "Input/InputListener.hpp"
#include "ControllableComponent.hpp"
#include "Core/GameObject.hpp"

#include <set>
#include <string>

namespace engine {

    class ControllableComponent : public Component, public InputListener {
    public:
        ControllableComponent();

        std::string getName() override;

        std::set<std::string> getRequiredComponents() override;

        void handleInput(const InputEvent& event) override;
    protected:
    };
}