#include "Game.hpp"
#include "Utils.hpp"

constexpr auto TIME_CONVERSION_FACTOR = 1000000000;

namespace engine {
	const void Game::run() {
		if (getTargetFrameRate() > 0 && Utils::getTimestampNS() - lastFrameTime < TIME_CONVERSION_FACTOR / (long long)getTargetFrameRate()) {
			return;
		}

		frameCount++;
		lastFrameTime = Utils::getTimestampNS();
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