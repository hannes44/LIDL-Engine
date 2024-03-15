#pragma once

#include "Core/Game.hpp"
#include <vector>
#include <memory>
#include <map>
#include "Components/CameraComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "GamePhysicsSettings.hpp"

#include <mutex>

namespace engine
{
	constexpr auto PHYSICS_TIME_CONVERSION_FACTOR = 1000000;

	class GamePhysics
	{
	protected:
		GamePhysics() {};

		void sendMultiplayerState(Game* game);
		std::string MULTIPLAYER_STATE_FOLDER = "../../../MultiplayerStates/";
		std::mutex multiplayerSendLock{};
		int multiplayerStateUpdateIntervalMS = 25;
		long long lastMultiplayerStateUpdateTimestamp = 0;

		inline static GamePhysics* instance;
		long long lastPhysicsUpdateTimestamp = 0;

	public:
		// Non cloneable
		GamePhysics(GamePhysics& other) = delete;
		// Non copyable
		void operator=(const GamePhysics&) = delete;

		static GamePhysics& getInstance();

		void fixedUpdate(const std::set<std::shared_ptr<GameObject>> gameObjects, GamePhysicsSettings& settings);
		void checkCollisions(Game* game, std::map<std::string, std::shared_ptr<GameObject>> gameObjects, GamePhysicsSettings& settings);
		std::pair<glm::vec3, glm::vec3> resolveCollisionFullyElastic(std::shared_ptr<PhysicsComponent> pc1, std::shared_ptr<PhysicsComponent> pc2);
		std::pair<glm::vec3, glm::vec3> resolveCollisionFullyInelastic(std::shared_ptr<PhysicsComponent> pc1, std::shared_ptr<PhysicsComponent> pc2);
		void run(Game* game);
	};
}
