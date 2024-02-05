#include "PhysicsComponent.hpp"
#include "ControllableComponent.hpp"
#include "Core/InputSystem.hpp"
#include "Core/Logger.hpp"
#include "Core/InputFramework.hpp"

#include <set>
#include <string>

namespace engine {

    ControllableComponent::ControllableComponent() {
         InputFramework::getInstance().addListener(this);
    }

    std::set<std::string> ControllableComponent::getRequiredComponents() {
        return {"Physics"};
    }

    std::string ControllableComponent::getName() {
        return "Controllable";
    }
    
    void ControllableComponent::handleInput(const InputEvent& event, const std::string& eventType) {
        int dir = 0;
        LOG_INFO("KEY PRESSED 1");
        if (eventType == "KeyUp")
            dir = 1;
        else if (eventType == "KeyDown")
            dir = -1;
        else
            return;

        LOG_INFO("KEY PRESSED");

        auto physicsComponent = gameObject->getComponent<PhysicsComponent>();
        
        if (event.getKey() == Key::UP) {
            physicsComponent->applyAcceleration(glm::vec3(0, dir, 0));
        }

        else if (event.getKey() == Key::DOWN) {
            physicsComponent->applyAcceleration(glm::vec3(0, -dir, 0));
        }

        else if (event.getKey() == Key::LEFT) {
            physicsComponent->applyAcceleration(glm::vec3(dir, 0, 0));
        }

        else if (event.getKey() == Key::RIGHT) {
            physicsComponent->applyAcceleration(glm::vec3(-dir, 0, 0));
        }
	}
}