#include "Game.hpp"
#include <chrono>

constexpr auto TIME_CONVERSION_FACTOR = 1000000000;

namespace engine {
	long long Game::getTime() {
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}

	const void Game::run() {
		if (getTargetFrameRate() > 0 && Game::getTime() - lastFrameTime < TIME_CONVERSION_FACTOR / (long long)getTargetFrameRate()) {
			return;
		}

		frameCount++;
		lastFrameTime = Game::getTime();
		update();
	}
	std::weak_ptr<Texture> Game::loadTexture(const std::string& textureFileName)
	{
		if (textures.count(textureFileName))
			return textures[textureFileName];

		std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::create(textureFileName));
		textures[textureFileName] = texture;
		return texture;
	}

	void Game::addGameObject(std::unique_ptr<GameObject> gameObject)
	{
		gameObjects[gameObject->uuid.id] = std::move(gameObject);
	}
}