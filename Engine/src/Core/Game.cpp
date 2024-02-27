#include "Game.hpp"
#include "Utils/Utils.hpp"
#include "Renderer/Renderer.hpp"
#include "Input/InputFramework.hpp"
#include "Window.hpp"
#include "Physics/GamePhysics.hpp"
#include "Core/Logger.hpp"
#include "Components/ColliderComponent.hpp"

constexpr auto TIME_CONVERSION_FACTOR = 1000000000;

namespace engine {
	const void Game::run() {
		if (getTargetFrameRate() > 0 && Utils::getTimestampNS() - lastFrameTime < TIME_CONVERSION_FACTOR / (long long)getTargetFrameRate()) {
			return;
		}

		frameCount++;
		lastFrameTime = Utils::getTimestampNS();

		InputFramework::getInstance().getInput();

		Renderer::getInstance()->renderGame(this, getMainCamera(), &renderingSettings);

		Window::getInstance().newFrame();

		GamePhysics::getInstance().run(this);
		update();
	}
	const void Game::gameLoop()
	{
		while (true)
		{
			run();
		}
	}

	std::weak_ptr<Texture> Game::loadTexture(const std::string& textureFileName)
	{
		if (textures.count(textureFileName))
			return textures[textureFileName];

		std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::create(textureFileName));
		textures[textureFileName] = texture;
		return texture;
	}

	void Game::resetGameState()
	{
		gameObjects.clear();
		materials.clear();
		textures.clear();
	}

	std::weak_ptr<Selectable> Game::getSelectable(const std::string& id)
	{
		if (gameObjects.count(id))
			return gameObjects[id];
		else if (materials.count(id))
			return materials[id];
		else if (textures.count(id))
			return textures[id];
		return std::weak_ptr<Selectable>();
	}

	void Game::addGameObject(std::shared_ptr<GameObject> gameObject)
	{
		gameObject->game = this;
		gameObjects[gameObject->uuid.id] = std::move(gameObject);
	}

	std::weak_ptr<GameObject> Game::getGameObject(const std::string& id)
	{
		if (gameObjects.count(id))
			return gameObjects[id];

		return std::weak_ptr<GameObject>();
	}

	void Game::deleteGameObject(const std::string& id)
	{
		gameObjects.erase(id);
	}

	void Game::deleteGameObject(GameObject* gameObject)
	{
		gameObjects.erase(gameObject->uuid.id);
	}

	void Game::addTexture(std::shared_ptr<Texture> texture)
	{
		textures[texture->getUUID().id] = texture;
	}

	std::weak_ptr<Texture> Game::getTexture(const std::string& id)
	{
		if (textures.count(id))
			return textures[id];
		return std::weak_ptr<Texture>();
	}

	void Game::deleteTexture(const std::string& id)
	{
		textures.erase(id);
	}

	void Game::addMaterial(std::shared_ptr<Material> material)
	{
		materials[material->uuid.id] = material;
	}

	std::weak_ptr<Material> Game::getMaterial(const std::string& id)
	{
		if (materials.count(id))
			return materials[id];
		return std::weak_ptr<Material>();
	}

	void Game::deleteMaterial(const std::string& id)
	{
		materials.erase(id);
	}

	void Game::changeMainCamera(GameObject* newCamera)
	{
		if (newCamera->getComponent<CameraComponent>() == nullptr)
		{
			LOG_WARN("Trying to set a non-camera object as main camera");
			return;
		}

		for (auto& [id, gameObject] : gameObjects)
		{
			if (gameObject.get() == newCamera)
			{
				if (auto lockedMainCamera = mainCamera.lock())
				{
					lockedMainCamera->getComponent<CameraComponent>()->isMainCamera = false;

					mainCamera = gameObject;
				}
			}
		}
	}

	std::weak_ptr<Material> Game::createMaterial(const std::string& name)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->name = name;
		materials[material->uuid.id] = material;
		return std::weak_ptr<Material>(material);
	}

	std::weak_ptr<Texture> Game::createTexture(const std::string& fileName)
	{
		std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::create(fileName));
		textures[texture->getUUID().id] = texture;
		return texture;
	}

	std::weak_ptr<GameObject> Game::createGameObject(const std::string& name)
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
		gameObject->name = name;
		gameObjects[gameObject->uuid.id] = gameObject;
		return gameObject;
	}


	CameraComponent* Game::getMainCamera()
	{
		if (auto lockedMainCamera = mainCamera.lock())
		{
			return lockedMainCamera->getComponent<CameraComponent>().get();
		}

		// If the main camera is not set, return the first camera found
		for (auto& [id, gameObject] : gameObjects)
		{
			if (gameObject->getComponent<CameraComponent>() != nullptr)
			{
				mainCamera = gameObject;
				return gameObject->getComponent<CameraComponent>().get();
			}
		}

		return nullptr;
	}

	std::vector<std::shared_ptr<GameObject>> Game::checkRayCollisions(glm::vec3 origin, glm::vec3 direction) {
		std::vector<RayCollision> collisions{};
		for (auto& [id, gameObject] : gameObjects) {
			auto collision = checkRayCollision(gameObject, origin, direction);
			if (collision.collision)
				collisions.push_back(collision);
		}

		std::sort(collisions.begin(), collisions.end());

		std::vector<std::shared_ptr<GameObject>> collidingObjects{};
		for (auto& collision : collisions)
			collidingObjects.push_back(collision.gameObject);

		return collidingObjects;
	}

	void Game::spawnClonedGameObjectFromTag(std::string tag)
	{
		LOG_INFO("Spawning object with tag: " + tag);
		for (auto& [id, gameObject] : gameObjects)
		{
			if (gameObject->tag == tag)
			{
				GameObject newGameObject = gameObject->clone();
				newGameObject.uuid = UUID();
				newGameObject.name = gameObject->name + " (Clone)";
				gameObjects[newGameObject.uuid.id] = std::make_shared<GameObject>(newGameObject);
				break;
			}
		}
	}

	glm::vec3 vec3min(glm::vec3 a, glm::vec3 b) {
		return glm::vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
	}

	glm::vec3 vec3max(glm::vec3 a, glm::vec3 b) {
		return glm::vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
	}

	RayCollision Game::checkRayCollision(std::shared_ptr<GameObject> gameObject, glm::vec3 origin, glm::vec3 direction) {
		auto collider = gameObject->getComponent<ColliderComponent>();
		if (collider == nullptr)
			return { false, 0, 0, gameObject };

		direction = glm::normalize(direction);

		// TODO: Since we are checking multiple boxes with the same ray, this can be optimized
		// See https://gist.github.com/DomNomNom/46bb1ce47f68d255fd5d
		auto aabb = collider->getBoundingBox();
		glm::vec3 tMin = (aabb.getMin() - origin) / direction;
		glm::vec3 tMax = (aabb.getMax() - origin) / direction;

		glm::vec3 t1 = vec3min(tMin, tMax);
		glm::vec3 t2 = vec3max(tMin, tMax);

		float tNear = max(max(t1.x, t1.y), t1.z);
		float tFar = min(min(t2.x, t2.y), t2.z);

		return { tNear >= 0 && tNear < tFar, tNear, tFar, gameObject };
	}
}