#include "GamePhysics.hpp"
#include "Core/Logger.hpp"
#include "Core/Game.hpp"
#include <string>
#include <chrono>
#include "Components/PhysicsComponent.hpp"
#include "Components/ColliderComponent.hpp"
#include "Utils/Utils.hpp"
#include "glm/glm.hpp"

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

            glm::vec3 force = physicsComponent->getForce();

            // Apply gravity
            if (physicsComponent->enableGravity)
                force.y -= physicsComponent->overrideGravityCoefficient ? physicsComponent->gravityCoefficient : settings.gravity;
            
            const bool zeroResultantForce = glm::length(force) < 0.0001f;
            
            // Apply friction
            if (zeroResultantForce)
                force -= physicsComponent->getVelocity() * 0.5f;

            physicsComponent->currentAcceleration = force / physicsComponent->mass;

            // Update the velocity, but snap to 0 if it is close to 0
            if (zeroResultantForce && glm::length(physicsComponent->getVelocity()) < 0.1f)
                physicsComponent->setVelocity(glm::vec3(0));
            else    
                physicsComponent->applyVelocity(physicsComponent->currentAcceleration * getFixedUpdateScale());
            
            // Move the object, scaled to account for the fixed update
			gameObject->transform.shiftPosition(physicsComponent->getVelocity() * getFixedUpdateScale());
        }
	}

    void GamePhysics::checkCollisions(std::map<std::string, std::shared_ptr<GameObject>> gameObjects) {
        std::vector<std::pair<std::string, std::string>> detectedCollisions{};
        
        // TODO: Change this loop to only check the combinations of GameObjects, so we can simplify some checks and avoid unnecessary iterations
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
                    colliderComponent1->isCurrentlyColliding = true;
                    colliderComponent2->isCurrentlyColliding = true;
                    LOG_TRACE("Collision detected between " + firstName + " and " + secondName);
                }
                else {
                    colliderComponent1->isCurrentlyColliding = false;
                    colliderComponent2->isCurrentlyColliding = false;
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