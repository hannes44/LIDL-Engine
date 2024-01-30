#pragma once

#include "Game.hpp"
#include "GameConfig.hpp"
#include <yaml-cpp/yaml.h>
#include "Material.hpp"
#include "Texture.hpp"

namespace engine
{
	#define GAME_FOLDER_PATH "../../games/"
	#define GAME_CONFIG_FILE_EXTENSION ".yaml"

	class GameSerializer
	{
	public:
		static void serializeGame(Game* game);

		static std::shared_ptr<Game> deserializeGame(const std::string& gameName);

		static void createYAMLFile(const std::string& filePath, const std::string& fileName);

	private:
		static void serializeGameConfig(const std::string& filePath, const Game* game);

		static void serializeGameState(const std::string& filePath, const Game* game);

		static void serializeGameObjects(const Game* game, YAML::Emitter& emitter);
		static void serializeGameObject(GameObject* gameObject, YAML::Emitter& emitter);

		static void serializeComponents(std::vector<std::shared_ptr<Component>> components, YAML::Emitter& emitter);
		static void serializeComponent(std::shared_ptr<Component>, YAML::Emitter& emitter);

		static void serializeTextures(const Game* games, YAML::Emitter& emitter);
		static void serializeTexture(Texture* texture, YAML::Emitter& emitter);

		static void serializeMaterials(const Game* game, YAML::Emitter& emitter);
		static void serializeMaterial(Material* material, YAML::Emitter& emitter);
		
		static void deserializeGameConfig(const std::string& gameName, Game* game);

		static void deserializeGameState(const std::string& gameName, Game* game);

		static void deserializeTextures(YAML::Node node, Game* game);

		static void deserializeMaterials(YAML::Node node, Game* game);

		static void deserializeGameObjects(YAML::Node node, Game* game);
		static void deserializeComponents(YAML::Node node, GameObject* gameObject);

		static void deserializeComponent(YAML::Node node, GameObject* gameObject);

	};
}