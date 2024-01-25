#pragma once
#include "Camera.hpp"
#include <memory>
#include <vector>
#include "Texture.hpp"
#include <string>
#include <map>
#include "GameConfig.hpp"

namespace engine
{
	class Game
	{
	public:
		// Called every frame
		virtual void update() = 0;

		// Called at initialization
		virtual void initialize() = 0;

		bool running = true;

		const void gameLoop();

		// Currenly limit the game to only one camera
		Camera camera{};

		std::map<std::string, std::shared_ptr<Texture>> textures{};

		std::shared_ptr<Texture> loadTexture(const std::string& textureFileName);

		std::string name = "Giga Game";

		GameConfig config{};

	protected:
		// 0 is uncapped
		virtual double getTargetFrameRate() = 0;

		int frameCount = 0;
		long long lastFrameTime = 0;

		static long long getTime();
	};
}