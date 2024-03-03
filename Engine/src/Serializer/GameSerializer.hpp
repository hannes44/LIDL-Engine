#pragma once

#include "Core/Game.hpp"
#include "Core/GameConfig.hpp"
#include <yaml-cpp/yaml.h>
#include "Core/Material.hpp"
#include "Core/Texture.hpp"

namespace engine
{
#define GAME_FOLDER_PATH "../../games/"
#define GAME_CONFIG_FILE_EXTENSION ".yaml"

	class GameSerializer
	{
	public:
		static void serializeGame(Game* game);

		// Reads the game config files and alters the game object accordingly
		static void deserializeGame(Game* game);

		static void createYAMLFile(const std::string& filePath, const std::string& fileName);
		static void createYAMLFile(const std::string& filePath);

		static void serializeSerializable(Serializable* serializable, YAML::Emitter& out);

		static std::string serializeGameState(const std::string& folderPath, const Game* game, bool serializeForMultiplayer = false);

		static void deserializeSerializable(YAML::Node node, Serializable* serializable);

		static void deserializeGameState(Game* game, std::string filePath);

	private:
		static void serializeGameConfig(const std::string& filePath, const Game* game);

		static void serializeGameObjects(const Game* game, YAML::Emitter& emitter, bool serializeForMultiplayer = false);
		static void serializeGameObject(GameObject* gameObject, YAML::Emitter& emitter, bool serializeForMultiplayer = false);

		static void serializeComponents(std::vector<std::shared_ptr<Component>> components, YAML::Emitter& emitter, bool serializeForMultiplayer = false);
		static void serializeComponent(std::shared_ptr<Component>, YAML::Emitter& emitter, bool serializeForMultiplayer = false);

		static void serializeTextures(const Game* games, YAML::Emitter& emitter);
		static void serializeTexture(Texture* texture, YAML::Emitter& emitter);

		static void serializeMaterials(const Game* game, YAML::Emitter& emitter, bool serializeForMultiplayer = false);
		static void serializeMaterial(Material* material, YAML::Emitter& emitter, bool serializeForMultiplayer = false);

		static void serializeActions(const Game* game, YAML::Emitter& emitter);

		static void deserializeGameConfig(Game* game);

		static void deserializeGameState(Game* game);

		static void deserializeTextures(YAML::Node node, Game* game);

		static void deserializeMaterials(YAML::Node node, Game* game);

		static void deserializeActions(YAML::Node node, Game* game);

		static void deserializeGameObjects(YAML::Node node, Game* game);
		static void deserializeComponents(YAML::Node node, GameObject* gameObject, Game* game);

		static void deserializeComponent(YAML::Node node, GameObject* gameObject, Game* game);

	};
}