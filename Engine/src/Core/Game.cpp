#include "Game.hpp"
#include "Utils/Utils.hpp"
#include "Renderer/Renderer.hpp"
#include "InputFramework.hpp"
#include "Window.hpp"
#include "Physics/GamePhysics.hpp"
#include "Core/Logger.hpp"

constexpr auto TIME_CONVERSION_FACTOR = 1000000000;

namespace engine {
	const void Game::run() {
		if (getTargetFrameRate() > 0 && Utils::getTimestampNS() - lastFrameTime < TIME_CONVERSION_FACTOR / (long long)getTargetFrameRate()) {
			return;
		}

		frameCount++;
		lastFrameTime = Utils::getTimestampNS();

		InputFramework::getInstance().getInput();

		Renderer::renderGame(this, getMainCamera(), &renderingSettings);

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

	void Game::addGameObject(std::shared_ptr<GameObject> gameObject)
	{
		gameObjects[gameObject->uuid.id] = std::move(gameObject);
	}

	void Game::deleteGameObject(const std::string& id)
	{
		gameObjects.erase(id);
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
}