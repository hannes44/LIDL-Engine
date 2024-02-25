#pragma once

#include "Core/Game.hpp"
#include <vector>
#include <memory>
#include <map>
#include "Components/CameraComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "GamePhysicsSettings.hpp"

namespace engine
{
	constexpr auto PHYSICS_TIME_CONVERSION_FACTOR = 1000000;

	class GamePhysics
	{
	protected:
		GamePhysics() {};

		inline static GamePhysics* instance;
		long long lastPhysicsUpdateTimestamp = 0;

	public:
		// Non cloneable
		GamePhysics(GamePhysics& other) = delete;
		// Non copyable
		void operator=(const GamePhysics&) = delete;

		static GamePhysics& getInstance();

		void fixedUpdate(std::map<std::string, std::shared_ptr<GameObject>> gameObjects, GamePhysicsSettings& settings);
		void checkCollisions(Game* game, std::map<std::string, std::shared_ptr<GameObject>> gameObjects, GamePhysicsSettings& settings);
		std::pair<glm::vec3, glm::vec3> resolveCollisionFullyElastic(std::shared_ptr<PhysicsComponent> pc1, std::shared_ptr<PhysicsComponent> pc2);
		std::pair<glm::vec3, glm::vec3> resolveCollisionFullyInelastic(std::shared_ptr<PhysicsComponent> pc1, std::shared_ptr<PhysicsComponent> pc2);
		void sendMultiplayerState(Game* game);
		void run(Game* game);

		// Disabled for now. TODO: Implement
		// void incrementalUpdate();

		GamePhysicsSettings& getSettings();
	};
}
