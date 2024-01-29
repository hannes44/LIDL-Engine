#pragma once

#include "Game.hpp"
#include <vector>
#include <memory>
#include <map>

namespace engine
{
    constexpr auto PHYSICS_TIME_CONVERSION_FACTOR = 1000000;

    struct GamePhysicsSettings {
        bool useGravity = true;
        float gravity = 9.82f;
        int fixedUpdateIntervalMS = 1000;
    };

	class GamePhysics
	{
	protected:
		GamePhysics() {};

		inline static GamePhysics* instance;
        GamePhysicsSettings settings;
        long long lastFrameTime = 0;

        /** 
         * The scale of the fixed update to correctly scale the physics.
         * If the fixed update is every second, the scale will be 1.
         * If the fixed update is twice a second, the scale will be 0.5 since a cube with the velocity 1 unit per second should only move 0.5 units each update.
        */
        float getFixedUpdateScale();
        static long long getTime();

	public:
		// Non cloneable
		GamePhysics(GamePhysics& other) = delete;
		// Non copyable
		void operator=(const GamePhysics&) = delete;

		static GamePhysics& getInstance();

        void fixedUpdate(std::map<std::string, std::shared_ptr<GameObject>> gameObjects);
        void run(Game *game);

        // Disabled for now. TODO: Implement
        // void incrementalUpdate();

        GamePhysicsSettings& getSettings();
	};
}
