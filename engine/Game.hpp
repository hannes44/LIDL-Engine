#pragma once
#include "Camera.hpp"
#include <memory>
#include <vector>
#include "Texture.hpp"
#include <string>
#include <map>
#include "GameConfig.hpp"
#include "Material.hpp"
#include "GameObject.hpp"
#include "RendererSettings.hpp"

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

		const void run();

		// Currenly limit the game to only one camera
		Camera camera{};

		// Texture Id to Texture
		std::map<std::string, std::shared_ptr<Texture>> textures{};

		// GameObject Id to GameObject
		std::map<std::string, std::shared_ptr<GameObject>> gameObjects{};

		// Material Id to Material
		std::map<std::string, std::shared_ptr<Material>> materials{};

		std::weak_ptr<Texture> loadTexture(const std::string& textureFileName);

		std::string name = "Giga Game";

		GameConfig config{};

		void addGameObject(std::unique_ptr<GameObject> gameObject);

		RendererSettings renderingSettings{};

	protected:
		// 0 is uncapped
		virtual double getTargetFrameRate() = 0;

		int frameCount = 0;
		long long lastFrameTime = 0;

		static long long getTime();
	};
}