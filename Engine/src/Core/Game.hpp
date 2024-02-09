#pragma once
#include "Components/CameraComponent.hpp"
#include <memory>
#include <vector>
#include "Texture.hpp"
#include <string>
#include <map>
#include "GameConfig.hpp"
#include "Material.hpp"
#include "GameObject.hpp"
#include "Renderer/RendererSettings.hpp"


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

		const void gameLoop();

		// Currenly limit the game to only one camera
		CameraComponent camera{};

		std::weak_ptr<Texture> loadTexture(const std::string& textureFileName);

		std::string name = "Giga Game";

		GameConfig config{};

		std::weak_ptr<GameObject> mainCamera;

		const std::map<std::string, std::shared_ptr<GameObject>> getGameObjects() const { return gameObjects; };

		const std::map<std::string, std::shared_ptr<Texture>> getTextures() const { return textures; };

		const std::map<std::string, std::shared_ptr<Material>> getMaterials() const { return materials; };

		// Returns a selectable object with the given id
		std::weak_ptr<Selectable> getSelectable(const std::string& id);

		void addGameObject(std::shared_ptr<GameObject> gameObject);
		std::weak_ptr<GameObject> getGameObject(const std::string& id);
		void deleteGameObject(const std::string& id);

		void addTexture(std::shared_ptr<Texture> texture);
		std::weak_ptr<Texture> getTexture(const std::string& id);
		void deleteTexture(const std::string& id);

		void addMaterial(std::shared_ptr<Material> material);
		std::weak_ptr<Material> getMaterial(const std::string& id);
		void deleteMaterial(const std::string& id);

		void changeMainCamera(GameObject* newCamera);

		std::weak_ptr<Material> createMaterial(const std::string& name);

		CameraComponent* getMainCamera();

		RendererSettings renderingSettings{};

	protected:
		// 0 is uncapped
		virtual double getTargetFrameRate() = 0;

		int frameCount = 0;
		long long lastFrameTime = 0;

		// Texture Id to Texture
		std::map<std::string, std::shared_ptr<Texture>> textures{};

		// GameObject Id to GameObject
		std::map<std::string, std::shared_ptr<GameObject>> gameObjects{};

		// Material Id to Material
		std::map<std::string, std::shared_ptr<Material>> materials{};
	};

	Game * createGame();
}