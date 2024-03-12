#include "GamePhysics.hpp"
#include "Core/Logger.hpp"
#include "Core/Game.hpp"
#include <string>
#include <chrono>
#include "Components/PhysicsComponent.hpp"
#include "Components/ColliderComponent.hpp"
#include "Utils/Utils.hpp"
#include "glm/glm.hpp"
#include "math.h"
#include <Serializer/Serializable.hpp>
#include "Components/ControllableComponent.hpp"
#include "GamePhysicsSettings.hpp"

#include "Serializer/GameSerializer.hpp"
#include "MultiplayerClient/Client.hpp"
#include <fstream>
#include <filesystem>

namespace engine {

	GamePhysics& GamePhysics::getInstance()
	{
		if (instance == nullptr)
			instance = new GamePhysics();

		return *instance;
	}

	void GamePhysics::sendMultiplayerState(Game* game) {
		auto tid = std::this_thread::get_id();
		std::string folderPath = MULTIPLAYER_STATE_FOLDER + game->instanceId + "/";
		std::filesystem::create_directory(folderPath);
		multiplayerSendLock.lock();
		std::string filePath = GameSerializer::serializeGameState(folderPath, game, true);

		if (!std::ifstream(filePath).good()) {
			LOG_ERROR("File not found: " + filePath);
			return;
		}

		std::ostringstream text;
		std::ifstream in_file(filePath);

		text << in_file.rdbuf();
		std::string message = text.str();
		multiplayerSendLock.unlock();
		Client::QueueMessage({ ClientMessageType::StateUpdate, message });
	}

	void GamePhysics::fixedUpdate(const std::set<std::shared_ptr<GameObject>> gameObjects, GamePhysicsSettings& settings) {
		for (auto const& gameObject : gameObjects) {
			std::shared_ptr<PhysicsComponent> physicsComponent = gameObject->getComponent<PhysicsComponent>();
			if (!physicsComponent) {
				continue;
			}

			glm::vec3 directionForce = glm::vec3(0);
			glm::vec3 directionVelocity = glm::vec3(0);

			auto controllableComponent = gameObject->getComponent<ControllableComponent>();
			if (controllableComponent) {
				glm::vec3 vec = glm::vec3(0);

				std::set<Direction> currentDirections = controllableComponent->currentDirections;

				if (currentDirections.contains(Direction::Up) && currentDirections.contains(Direction::Down))
				{
					currentDirections.erase(Direction::Up);
					currentDirections.erase(Direction::Down);
				}
				else if (currentDirections.contains(Direction::Left) && currentDirections.contains(Direction::Right))
				{
					currentDirections.erase(Direction::Left);
					currentDirections.erase(Direction::Right);
				}
				else if (currentDirections.contains(Direction::Forward) && currentDirections.contains(Direction::Backward))
				{
					currentDirections.erase(Direction::Forward);
					currentDirections.erase(Direction::Backward);
				}

				for (auto direction : currentDirections) {
					switch (direction) {
					case Direction::Up:
						vec += controllableComponent->orientation * physicsComponent->up;
						break;
					case Direction::Down:
						vec -= controllableComponent->orientation * physicsComponent->up;
						break;
					case Direction::Left:
						vec -= controllableComponent->orientation * physicsComponent->right;
						break;
					case Direction::Right:
						vec += controllableComponent->orientation * physicsComponent->right;
						break;
					case Direction::Forward:
						vec += controllableComponent->orientation * physicsComponent->forward;
						break;
					case Direction::Backward:
						vec -= controllableComponent->orientation * physicsComponent->forward;
						break;
					}
				}

				if (controllableComponent->enableForces)
					physicsComponent->setForce(vec);
				else
					physicsComponent->setVelocity(vec);

				if (currentDirections.size() > 0)
					vec = glm::normalize(vec) * controllableComponent->movementSpeed;

				if (controllableComponent->enableForces)
					directionForce = vec;
				else
					directionVelocity = vec;
			}

			glm::vec3 force = physicsComponent->getForce() + directionForce;
			glm::vec3 velocity = physicsComponent->getVelocity() + directionVelocity;

			const bool applyFriction = settings.enableForces ? glm::length(force) < 0.0001f : true;

			// Apply gravity
			if (settings.enableGravity && physicsComponent->enableGravity)
				force.y -= physicsComponent->overrideGravityCoefficient ? physicsComponent->gravityCoefficient : settings.gravity;

			const bool zeroResultantForce = settings.enableForces ? glm::length(force) < 0.0001f : true;

			// Apply friction
			if (settings.enableFriction && applyFriction)
				force -= velocity * settings.frictionCoefficient;

			physicsComponent->currentAcceleration = force / physicsComponent->mass;

			// Update the velocity, but snap to 0 if it is close to 0
			if (zeroResultantForce && glm::length(velocity) < 0.1f)
				physicsComponent->setVelocity(glm::vec3(0));
			else
				physicsComponent->applyVelocity(physicsComponent->currentAcceleration * settings.getFixedUpdateScale());

			// Move the object, scaled to account for the fixed update
			gameObject->transform.shiftPosition(velocity * settings.getFixedUpdateScale());
		}
	}

	// In a fully elastic collision, the kinetic energy is conserved (they bounce off each other)
	std::pair<glm::vec3, glm::vec3> GamePhysics::resolveCollisionFullyElastic(std::shared_ptr<PhysicsComponent> pc1, std::shared_ptr<PhysicsComponent> pc2) {
		const glm::vec3 u1 = pc1->currentVelocity;
		const glm::vec3 u2 = pc2->currentVelocity;

		const glm::vec3 v1 = (pc1->mass - pc2->mass) / (pc1->mass + pc2->mass) * u1 + (2 * pc2->mass) / (pc1->mass + pc2->mass) * u2;
		const glm::vec3 v2 = 2 * pc1->mass / (pc1->mass + pc2->mass) * u1 + (pc2->mass - pc1->mass) / (pc1->mass + pc2->mass) * u2;

		return std::make_pair(v1, v2);
	}

	// In a fully inelastic collision, the kinetic energy is not conserved (they stick together)
	std::pair<glm::vec3, glm::vec3> GamePhysics::resolveCollisionFullyInelastic(std::shared_ptr<PhysicsComponent> pc1, std::shared_ptr<PhysicsComponent> pc2) {
		const glm::vec3 u1 = pc1->currentVelocity;
		const glm::vec3 u2 = pc2->currentVelocity;

		const glm::vec3 v = (pc1->mass * u1 + pc2->mass * u2) / (pc1->mass + pc2->mass);

		return std::make_pair(v, v);
	}

	void GamePhysics::checkCollisions(Game* game, std::map<std::string, std::shared_ptr<GameObject>> gameObjects, GamePhysicsSettings& settings) {
		if (!settings.enableCollisions)
			return;

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
					LOG_TRACE("Collision detected between " + firstName + " and " + secondName);
					if (colliderComponent1->onCollision)
						colliderComponent1->onCollision(game, colliderComponent2.get()->gameObject, colliderComponent2.get());
					if (colliderComponent2->onCollision)
						colliderComponent2->onCollision(game, colliderComponent1.get()->gameObject, colliderComponent1.get());

					detectedCollisions.push_back(std::make_pair(firstName, secondName));
					colliderComponent1->isCurrentlyColliding = true;
					colliderComponent2->isCurrentlyColliding = true;

					/* --- Temporary conversion until enums are serializable --- */
					CollisionResolveType collisionResolveType;
					if (settings.collisionResolveType == "FULLY_ELASTIC")
						collisionResolveType = CollisionResolveType::FULLY_ELASTIC;
					else if (settings.collisionResolveType == "FULLY_INELASTIC")
						collisionResolveType = CollisionResolveType::FULLY_INELASTIC;
					else if (settings.collisionResolveType == "DISABLED")
						collisionResolveType = CollisionResolveType::DISABLED;
					else
						LOG_FATAL("Invalid collision type");
					/* --- */

					// Do not resolve collisions if they are disabled
					if (collisionResolveType == CollisionResolveType::DISABLED)
						continue;

					auto pc1 = gameObject1->getComponent<PhysicsComponent>();
					auto pc2 = gameObject2->getComponent<PhysicsComponent>();

					// We cannot resolve collisions if both objects are static
					if (!pc1 && !pc2)
						continue;

					if (colliderComponent1->isPlatform && !colliderComponent2->isPlatform) {
						pc2->setVelocity(glm::vec3(pc2->currentVelocity.x, 0, pc2->currentVelocity.z));
						continue;
					}

					if (!colliderComponent1->isPlatform && colliderComponent2->isPlatform) {
						pc1->setVelocity(glm::vec3(pc1->currentVelocity.x, 0, pc1->currentVelocity.z));
						continue;
					}

					// If only one of the objects has a PhysicsComponent, reflect it
					if (pc1 && !pc2)
						pc1->setVelocity(-pc1->currentVelocity);
					else if (!pc1 && pc2)
						pc2->setVelocity(-pc2->currentVelocity);
					else {
						std::pair<glm::vec3, glm::vec3> finalVelocities;

						switch (collisionResolveType) {
						case FULLY_ELASTIC:
							finalVelocities = resolveCollisionFullyElastic(pc1, pc2);
							break;
						case FULLY_INELASTIC:
							finalVelocities = resolveCollisionFullyInelastic(pc1, pc2);
							break;
						}

						pc1->setVelocity(finalVelocities.first);
						pc2->setVelocity(finalVelocities.second);
					}
				}
				else {
					colliderComponent1->isCurrentlyColliding = false;
					colliderComponent2->isCurrentlyColliding = false;
				}
			}
		}
	}

	void GamePhysics::run(Game* game) {
		if (Utils::getTimestampNS() - lastPhysicsUpdateTimestamp < PHYSICS_TIME_CONVERSION_FACTOR * game->config.physicsSettings.fixedUpdateIntervalMS) {
			return;
		}

		lastPhysicsUpdateTimestamp = Utils::getTimestampNS();
		fixedUpdate(game->getRootGameObjects(), game->config.physicsSettings);
		checkCollisions(game, game->getGameObjects(), game->config.physicsSettings);
		
		if (game->isMultiplayerGame() && game->running && Utils::getTimestampNS() - lastMultiplayerStateUpdateTimestamp > 1000000 * multiplayerStateUpdateIntervalMS) {
			lastMultiplayerStateUpdateTimestamp = Utils::getTimestampNS();
			sendMultiplayerState(game);
		}
	}
}