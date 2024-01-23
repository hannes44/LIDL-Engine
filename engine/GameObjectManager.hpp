#pragma once

#include "GameObject.hpp"
#include <vector>
#include <memory>

namespace engine
{
	class GameObjectManager
	{
	protected:
		GameObjectManager() {};

		inline static GameObjectManager* instance;

	public:
		// Non cloneable
		GameObjectManager(GameObjectManager& other) = delete;
		// Non copyable
		void operator=(const GameObjectManager&) = delete;

		static GameObjectManager& getInstance();

		void update();

		void initialize();

		void addGameObject(GameObject* gameObject);

		std::vector<std::unique_ptr<GameObject>> gameObjects{};
	};
} // namespace engine
