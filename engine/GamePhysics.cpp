#include "GamePhysics.hpp"
#include "Logger.hpp"
#include "Game.hpp"
#include <string>
#include <chrono>
#include "PhysicsComponent.hpp"

namespace engine {

    GamePhysics& GamePhysics::getInstance()
	{
		if (instance == nullptr)
			instance = new GamePhysics();

		return *instance;
	}

    long long GamePhysics::getTime() {
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
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
        if (GamePhysics::getTime() - lastFrameTime < PHYSICS_TIME_CONVERSION_FACTOR * settings.fixedUpdateIntervalMS) {
            return;
        }

        lastFrameTime = GamePhysics::getTime();
        fixedUpdate(game->gameObjects);
    }
}