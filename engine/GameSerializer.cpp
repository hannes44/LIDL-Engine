#include "GameSerializer.hpp"
#include "TestGame.hpp"
#include "Logger.hpp"
#include <fstream>
#include <Windows.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

namespace engine
{
	#define GAME_FOLDER_PATH "../../games/"
	#define GAME_CONFIG_FILE_EXTENSION ".yaml"

	void GameSerializer::SerializeGame(Game& game)
	{
		LOG_INFO("Serializing game: " + game.name);
		std::string directoryFilePath = GAME_FOLDER_PATH + game.name;
		// Creates the game folder if it doesn't already exist
		CreateDirectory(directoryFilePath.c_str(), NULL);

		std::string insideDirectoryFilePath = directoryFilePath + "/";


		// Serialize the game config
		serializeGameConfig(insideDirectoryFilePath, game);

		serializeGameState(insideDirectoryFilePath, game);

		// Create YAML file for the initial game state

		// Save the scripts in the folder???
	}

	Game* GameSerializer::DeserializeGame()
	{
		return new TestGame();
	}

	void GameSerializer::createYAMLFile(const std::string& filePath, const std::string& fileName)
	{
		LOG_INFO("Creating YAML file: " + filePath + fileName + GAME_CONFIG_FILE_EXTENSION);
		std::ofstream outfile(filePath + fileName + GAME_CONFIG_FILE_EXTENSION);
		outfile.close();
	}
	
	void GameSerializer::serializeGameConfig(const std::string& filePath, const Game& game)
	{
		LOG_INFO("Serializing game config: " + game.name);
		std::string configFileName = game.name + "Config ";
		createYAMLFile(filePath, configFileName);


		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "name";
		out << YAML::Value << game.config.gameName;

		out << YAML::Key << "isDefaultFullscreen";
		out << YAML::Value << game.config.isDefaultFullscreen;
		
		out << YAML::Key << "graphicsAPI";
		out << YAML::Value << (game.config.graphicsAPIType == GraphicsAPIType::OpenGL ? "OpenGL" : "UNKNOWN");
		
		out << YAML::EndMap;

		std::ofstream fout(filePath + configFileName + GAME_CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		std::cout << "Here's the output YAML:\n" << out.c_str(); // prints "Hello, World!"

		LOG_INFO("Serialized game config: " + game.name);
	}

	void GameSerializer::serializeGameState(const std::string& filePath, const Game& game)
	{
		LOG_INFO("Serializing game state: " + game.name);
		std::string configFileName = game.name + "State ";
		createYAMLFile(filePath, configFileName);

		YAML::Emitter out;


		serializeGameObjects({}, out);



		std::ofstream fout(filePath + configFileName + GAME_CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		std::cout << "Here's the output YAML:\n" << out.c_str(); // prints "Hello, World!"

		LOG_INFO("Serialized game state: " + game.name);
	}
	void GameSerializer::serializeGameObjects(const std::vector<GameObject*> gameObject, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "GameObjects";
		out << YAML::Value << YAML::BeginSeq;
		serializeGameObject(nullptr, out);
		serializeGameObject(nullptr, out);
		out	<< YAML::EndSeq;
		out << YAML::EndMap;

	}
	void GameSerializer::serializeGameObject(GameObject* gameObject, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "name";
		out << YAML::Value << "GameObject1";
		out << YAML::Key << "position";
		out << YAML::Value << "1,1,1";
		out << YAML::EndMap;
	}
	void GameSerializer::serializeTextures(const std::vector<Texture*> textures, YAML::Emitter& out)
	{
	}
	void GameSerializer::serializeMaterials(const std::vector<Material*> materials, YAML::Emitter& out)
	{
	}
}
