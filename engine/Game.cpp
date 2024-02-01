#include "Game.hpp"
#include "Utils.hpp"
#include "Renderer.hpp"
#include "InputFramework.hpp"
#include "Window.hpp"

constexpr auto TIME_CONVERSION_FACTOR = 1000000000;

namespace engine {
	const void Game::run() {
		if (getTargetFrameRate() > 0 && Utils::getTimestampNS() - lastFrameTime < TIME_CONVERSION_FACTOR / (long long)getTargetFrameRate()) {
			return;
		}

		frameCount++;
		lastFrameTime = Utils::getTimestampNS();

		InputFramework::getInstance().getInput();

		Renderer::renderGame(this, &camera, &renderingSettings);

		Window::getInstance().newFrame();
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
}