#include "GamePhysics.hpp"
#include "Core/Logger.hpp"
#include "Core/Game.hpp"
#include <string>
#include <chrono>
#include "Components/PhysicsComponent.hpp"
#include "Utils/Utils.hpp"

namespace engine {

    GamePhysics& GamePhysics::getInstance()
	{
		if (instance == nullptr)
			instance = new GamePhysics();

		return *instance;
	}

    float GamePhysics::getFixedUpdateScale() {
        return settings.fixedUpdateIntervalMS / 1000.0f;
    }

	void GamePhysics::fixedUpdate(std::map<std::string, std::shared_ptr<GameObject>> gameObjects) {
        for (auto const& [name, gameObject] : gameObjects) {
            std::shared_ptr<PhysicsComponent> physicsComponent = gameObject->getComponent<PhysicsComponent>();
            if (!physicsComponent) {
                continue;
            }

            glm::vec3 velocity = physicsComponent->getVelocity();

            // Apply gravity
            if (!physicsComponent->disableGravity) {
                velocity.y -= physicsComponent->overrideGravityCoefficient ? physicsComponent->gravityCoefficient : settings.gravity;
            }
            
            // Move the object, scaled to account for the fixed update
			gameObject->transform.shiftPosition(velocity * getFixedUpdateScale());
        }
	}

    void GamePhysics::run(Game *game) {
        if (Utils::getTimestampNS() - lastPhysicsUpdateTimestamp < PHYSICS_TIME_CONVERSION_FACTOR * settings.fixedUpdateIntervalMS) {
            return;
        }

        lastPhysicsUpdateTimestamp = Utils::getTimestampNS();
        fixedUpdate(game->gameObjects);
    }
}