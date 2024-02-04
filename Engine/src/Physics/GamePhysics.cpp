#include "GamePhysics.hpp"
#include "Core/Logger.hpp"
#include "Core/Game.hpp"
#include <string>
#include <chrono>
#include "Components/PhysicsComponent.hpp"
#include "Components/ColliderComponent.hpp"
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
            if (physicsComponent->enableGravity) {
                velocity.y -= physicsComponent->overrideGravityCoefficient ? physicsComponent->gravityCoefficient : settings.gravity;
            }
            
            // Move the object, scaled to account for the fixed update
			gameObject->transform.shiftPosition(velocity * getFixedUpdateScale());
        }
	}

    void GamePhysics::checkCollisions(std::map<std::string, std::shared_ptr<GameObject>> gameObjects) {
        std::vector<std::pair<std::string, std::string>> detectedCollisions{};
        
        for (auto const& [name1, gameObject1] : gameObjects) {
            std::shared_ptr<ColliderComponent> colliderComponent1 = gameObject1->getComponent<ColliderComponent>();
            if (!colliderComponent1)
                continue;

            for (auto const& [name2, gameObject2] : gameObjects) {
                // Do not check collisions with itself
                if (name1 == name2)
                    continue;

                // Sort the names to get the same output for collisions between the same objects
                std::string firstName = name1 < name2 ? name1 : name2;
                std::string secondName = name1 < name2 ? name2 : name1;
                
                // Do not check already detected collisions
                if (std::find(detectedCollisions.begin(), detectedCollisions.end(), std::make_pair(firstName, secondName)) != detectedCollisions.end())
                    continue;

                std::shared_ptr<ColliderComponent> colliderComponent2 = gameObject2->getComponent<ColliderComponent>();
                if (!colliderComponent2)
                    continue;

                if (colliderComponent1->isColliding(colliderComponent2.get())) {
                    detectedCollisions.push_back(std::make_pair(firstName, secondName));
                    LOG_TRACE("Collision detected between " + firstName + " and " + secondName);
                }
            }
        }
    }

    void GamePhysics::run(Game *game) {
        if (Utils::getTimestampNS() - lastPhysicsUpdateTimestamp < PHYSICS_TIME_CONVERSION_FACTOR * settings.fixedUpdateIntervalMS) {
            return;
        }

        lastPhysicsUpdateTimestamp = Utils::getTimestampNS();
        checkCollisions(game->gameObjects);
        fixedUpdate(game->gameObjects);
    }
}