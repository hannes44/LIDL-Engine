#pragma once

#include "Components/CameraComponent.hpp"
#include <memory>
#include <vector>
#include "Texture.hpp"
#include <string>
#include <map>
#include <set>
#include "GameConfig.hpp"
#include "Material.hpp"
#include "GameObject.hpp"
#include "Renderer/RendererSettings.hpp"

namespace engine
{
	struct RayCollision
	{
		bool collision = false;
		float nearCollision = 0;
		float farCollision = 0;
		std::shared_ptr<GameObject> gameObject;

		bool operator < (const RayCollision& other) const
		{
			return nearCollision < other.nearCollision;
		}
	};

	class Game
	{
	public:
		Game();
		// Called every frame
		virtual void update() = 0;

		// Called at initialization
		virtual void initialize() = 0;

		const void run();

		const void gameLoop();

		const std::map<std::string, std::shared_ptr<GameObject>> getGameObjects() const { return gameObjects; };
		const std::set<std::shared_ptr<GameObject>> getRootGameObjects() const;
		std::weak_ptr<Texture> loadTexture(const std::string& textureFileName);
		const std::map<std::string, std::shared_ptr<Texture>> getTextures() const { return textures; };
		const std::map<std::string, std::shared_ptr<Material>> getMaterials() const { return materials; };

		// Deletes all game objects, textures, and materials
		void resetGameState();

		// Returns a selectable object with the given id
		std::weak_ptr<Selectable> getSelectable(const std::string& id);

		void addGameObject(std::shared_ptr<GameObject> gameObject);
		std::weak_ptr<GameObject> getGameObject(const std::string& id);
		void deleteGameObjectFromId(const std::string& id);
		void deleteGameObject(GameObject* gameObject);

		void addTexture(std::shared_ptr<Texture> texture);
		std::weak_ptr<Texture> getTexture(const std::string& id);
		void deleteTexture(const std::string& id);

		void addMaterial(std::shared_ptr<Material> material);
		std::weak_ptr<Material> getMaterial(const std::string& id);
		void deleteMaterial(const std::string& id);

		std::weak_ptr<Material> createMaterial(const std::string& name);
		std::weak_ptr<Texture> createTexture(const std::string& name);
		std::weak_ptr<GameObject> createGameObject(const std::string& name);

		CameraComponent* getMainCamera();
		void changeMainCamera(GameObject* newCamera);

		virtual bool isMultiplayerGame() { return false; }
		
		/// Returns a vector of the GameObjects that collided with the ray, sorted from closest to farthest
		std::vector<std::shared_ptr<GameObject>> checkRayCollisions(glm::vec3 origin, glm::vec3 direction);
		std::string getIdOfGameObjectHitByRay(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

		// Clones a game object from a gameobject with the given tag
		void spawnClonedGameObjectFromTag(const std::string& tag);
		void setParent(std::shared_ptr<GameObject> gameObject, std::shared_ptr<GameObject> newParent);
		void removeParent(std::shared_ptr<GameObject> gameObject);
		std::string getTagOfGameObject(const std::string& id);
		int getNumberOfGameObjectsWithTag(const std::string& tag);
		std::tuple<float, float, float> getGameObjectPositionFromTag(const std::string& tag);
		void setGameObjectMeshVisibilityFromTag(const std::string& tag, bool visible);

		std::string name = "Giga Game";
		bool running = false;
		GameConfig config{};
		std::string instanceId = "";
		std::weak_ptr<GameObject> mainCamera;
		RendererSettings renderingSettings{};
	protected:
		RayCollision checkRayCollision(std::shared_ptr<GameObject> gameObject, glm::vec3 origin, glm::vec3 direction);
		// 0 is uncapped
		virtual double getTargetFrameRate() = 0;

		// Texture Id to Texture
		std::map<std::string, std::shared_ptr<Texture>> textures{};
		// GameObject Id to GameObject
		std::map<std::string, std::shared_ptr<GameObject>> gameObjects{};
		// Material Id to Material
		std::map<std::string, std::shared_ptr<Material>> materials{};

		std::set<std::string> gameObjectRootIDs{};

		int frameCount = 0;
		long long lastFrameTime = 0;
	};

	Game* createGame();
}