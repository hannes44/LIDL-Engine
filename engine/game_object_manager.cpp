#include "game_object_manager.hpp"

namespace engine
{
	GameObjectManager& GameObjectManager::getInstance()
	{
		if (instance == nullptr)
			instance = new GameObjectManager();

		return *instance;
	}

	void GameObjectManager::update()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->update();
		}
	}

	void GameObjectManager::initialize()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->initialize();
		}
	}

	void GameObjectManager::addGameObject(GameObject* gameObject)
	{
		gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
	}

}
