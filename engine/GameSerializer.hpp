#pragma once

#include "Game.hpp"
#include "GameConfig.hpp"
#include <yaml-cpp/yaml.h>
#include "Material.hpp"
#include "Texture.hpp"

namespace engine
{
	class GameSerializer
	{
	public:
		static void SerializeGame(Game& game);

		static Game* DeserializeGame();

		static void createYAMLFile(const std::string& filePath, const std::string& fileName);

	private:
		static void serializeGameConfig(const std::string& filePath, const Game& game);

		static void serializeGameState(const std::string& filePath, const Game& game);

		static void serializeGameObjects(const std::vector<GameObject*> gameObject, YAML::Emitter& emitter);

		static void serializeGameObject(GameObject* gameObject, YAML::Emitter& emitter);

		static void serializeTextures(const std::vector<Texture*> textures, YAML::Emitter& emitter);

		static void serializeMaterials(const std::vector<Material*> materials, YAML::Emitter& emitter);
		
	};
}