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
        int forceDir = 0;
        if (eventType == "KeyDown")
            forceDir = 1;
        else if (eventType == "KeyUp")
            forceDir = -1;
        else
            return;

        auto physicsComponent = gameObject->getComponent<PhysicsComponent>();
        const float strength = 3;
        
        if (event.getKey() == Key::LSHIFT) {
            physicsComponent->applyForce(glm::vec3(0, forceDir * strength, 0));
        }

        else if (event.getKey() == Key::LCTRL) {
            physicsComponent->applyForce(glm::vec3(0, -forceDir * strength, 0));
        }

        else if (event.getKey() == Key::A) {
            physicsComponent->applyForce(glm::vec3(-forceDir * strength, 0, 0));
        }

        else if (event.getKey() == Key::D) {
            physicsComponent->applyForce(glm::vec3(forceDir * strength, 0, 0));
        }

        else if (event.getKey() == Key::W) {
            physicsComponent->applyForce(glm::vec3(0, 0, -forceDir * strength));
        }

        else if (event.getKey() == Key::S) {
            physicsComponent->applyForce(glm::vec3(0, 0, forceDir * strength));
        }
	}
}