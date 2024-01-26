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
		std::string insideDirectoryFilePath = directoryFilePath + "/";

		// Creates the game folder if it doesn't already exist
		CreateDirectory(directoryFilePath.c_str(), NULL);

		// Serialize the game config
		serializeGameConfig(insideDirectoryFilePath, game);

		serializeGameState(insideDirectoryFilePath, game);
	}

	std::unique_ptr<Game> GameSerializer::DeserializeGame()
	{
		return std::make_unique<TestGame>();
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
		out << YAML::BeginMap;

		serializeGameObjects({}, out);
		serializeTextures({}, out);
		serializeMaterials({}, out);

		out << YAML::EndMap;
		std::ofstream fout(filePath + configFileName + GAME_CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		std::cout << "Here's the output YAML:\n" << out.c_str(); // prints "Hello, World!"

		LOG_INFO("Serialized game state: " + game.name);
	}
	void GameSerializer::serializeGameObjects(const std::vector<GameObject*> gameObjects, YAML::Emitter& out)
	{
		GameObject* gameObject = new GameObject();
		out << YAML::Key << "GameObjects";
		out << YAML::Value << YAML::BeginSeq;
		serializeGameObject(gameObject, out);
		serializeGameObject(gameObject, out);
		out	<< YAML::EndSeq;

		if (out.good())
			LOG_INFO("Serialized game objects");
		else
			LOG_ERROR("Failed to serialize game objects");

	}
	void GameSerializer::serializeGameObject(GameObject* gameObject, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "name";
		out << YAML::Value << gameObject->name;
		out << YAML::Key << "transform";
		Transform transform = gameObject->transform;
		float* mat4Pointer = &transform.transformMatrix[0][0];
		std::vector<float> mat4Vector(mat4Pointer, mat4Pointer + 16);
		out << YAML::Value << YAML::Flow << mat4Vector;
		out << YAML::Key << "isVisible";
		out << YAML::Value << gameObject->isVisible;
		out << YAML::Key << "Id";
		// TODO: Add proper Id when UUID is implemented
		out << YAML::Value << "ID TODO Add when implemented";
		out << YAML::EndMap;

		if (out.good())
			LOG_TRACE("Serialized game object: " + gameObject->name);
		else
			LOG_ERROR("Failed to serialize game object: " + gameObject->name);
	}
	void GameSerializer::serializeTextures(const std::vector<Texture*> textures, YAML::Emitter& out)
	{
		Texture* texture = Texture::create("bompaspy.png");
		out << YAML::Key << "Textures";
		out << YAML::Value << YAML::BeginSeq;
		serializeTexture(texture, out);
		out << YAML::EndSeq;

		if (out.good())
			LOG_INFO("Serialized textures");
		else
			LOG_ERROR("Failed to serialize textures");
	}
	void GameSerializer::serializeTexture(Texture* texture, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "name";
		out << YAML::Value << texture->name;
		out << YAML::Key << "fileName";
		out << YAML::Value << texture->filename;
		out << YAML::Key << "Id";
		// TODO: Add proper Id when UUID is implemented
		out << YAML::Value << "ID TODO Add when implemented";
		out << YAML::EndMap;

		if (out.good())
			LOG_TRACE("Serialized texture: " + texture->name);
		else
			LOG_ERROR("Failed to serialize texture: " + texture->name);
	}
	void GameSerializer::serializeMaterials(const std::vector<Material*> materials, YAML::Emitter& out)
	{
		Material* material = new Material();
		out << YAML::Key << "Materials";
		out << YAML::Value << YAML::BeginSeq;
		serializeMaterial(material, out);
		out << YAML::EndSeq;

		if (out.good())
			LOG_INFO("Serialized materials");
		else
			LOG_ERROR("Failed to serialize materials");
	}
	void GameSerializer::serializeMaterial(Material* material, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "name";
		out << YAML::Value << material->name;
		out << YAML::Key << "transparency";
		out << YAML::Value << material->transparency;
		out << YAML::Key << "emission";
		out << YAML::Value << YAML::Flow << material->emission[0];
		out << YAML::Key << "roughness";
		out << YAML::Value << material->roughness;
		out << YAML::Key << "shininess";
		out << YAML::Value << material->shininess;
		out << YAML::Key << "Id";
		// TODO: Add proper Id when UUID is implemented
		out << YAML::Value << "ID TODO Add when implemented";

		out << YAML::EndMap;

		if (out.good())
			LOG_TRACE("Serialized material: " + material->name);
		else
			LOG_ERROR("Failed to serialize material: " + material->name);
	}
}
