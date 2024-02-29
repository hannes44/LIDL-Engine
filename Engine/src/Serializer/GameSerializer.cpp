#include "GameSerializer.hpp"
#include "Core/Logger.hpp"
#include <fstream>
#include <Windows.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <memory>
#include "Components/MeshComponent.hpp"
#include "Components/PointLightComponent.hpp"
#include "Components/ComponentFactory.hpp"
#include "Serializable.hpp"
#include "Components/ScriptableComponent.hpp"

namespace engine
{

	// Serializes the game into a folder with config and state yaml files
	void GameSerializer::serializeGame(Game* game)
	{
		LOG_INFO("Serializing game: " + game->name);

		if (game->running)
		{
			LOG_WARN("Cannot serialize game while it is running");
			return;
		}

		std::string directoryFilePath = GAME_FOLDER_PATH + game->name;
		std::string insideDirectoryFilePath = directoryFilePath + "/";

		// Creates the game folder if it doesn't already exist
		CreateDirectory(directoryFilePath.c_str(), NULL);

		// Serialize the game config
		serializeGameConfig(insideDirectoryFilePath, game);

		serializeGameState(insideDirectoryFilePath, game);
	}

	void GameSerializer::createYAMLFile(const std::string& filePath, const std::string& fileName)
	{
		LOG_INFO("Creating YAML file: " + filePath + fileName + GAME_CONFIG_FILE_EXTENSION);
		std::ofstream outfile(filePath + fileName + GAME_CONFIG_FILE_EXTENSION);
		outfile.close();
	}

	// Serialized the game config to a YAML file at the given file path
	void GameSerializer::serializeGameConfig(const std::string& filePath, const Game* game)
	{
		LOG_INFO("Serializing game config: " + game->name);
		std::string configFileName = game->name + "Config";
		createYAMLFile(filePath, configFileName);


		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "name";
		out << YAML::Value << game->config.gameName;

		out << YAML::Key << "isDefaultFullscreen";
		out << YAML::Value << game->config.isDefaultFullscreen;

		out << YAML::Key << "graphicsAPI";
		out << YAML::Value << (game->config.graphicsAPIType == GraphicsAPIType::OpenGL ? "OpenGL" : "UNKNOWN");

		out << YAML::EndMap;

		std::ofstream fout(filePath + configFileName + GAME_CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		std::cout << "Here's the output YAML:\n" << out.c_str() << std::endl; // prints "Hello, World!"

		LOG_INFO("Serialized game config: " + game->name);
	}

	// Serializes the game state to a YAML file at the given file path
	void GameSerializer::serializeGameState(const std::string& filePath, const Game* game)
	{
		LOG_INFO("Serializing game state: " + game->name);
		std::string configFileName = game->name + "State";
		createYAMLFile(filePath, configFileName);

		YAML::Emitter out;
		out << YAML::BeginMap;

		serializeGameObjects(game, out);
		serializeTextures(game, out);
		serializeMaterials(game, out);

		out << YAML::EndMap;
		std::ofstream fout(filePath + configFileName + GAME_CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		std::cout << "Here's the output YAML:\n" << out.c_str() << std::endl; // prints "Hello, World!"

		LOG_INFO("Serialized game state: " + game->name);
	}

	// Serializes all game objects to the given YAML emitter, will create a sequence of game objects
	void GameSerializer::serializeGameObjects(const Game* game, YAML::Emitter& out)
	{
		out << YAML::Key << "GameObjects";
		out << YAML::Value << YAML::BeginSeq;

		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			serializeGameObject(gameObject.get(), out);
		}

		out << YAML::EndSeq;

		if (out.good())
			LOG_INFO("Serialized game objects");
		else
			LOG_ERROR("Failed to serialize game objects");

	}

	// Serializes a game object to the given YAML emitter as a map
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
		out << YAML::Value << gameObject->uuid.id;
		out << YAML::Key << "tag";
		out << YAML::Value << gameObject->tag;

		serializeComponents(gameObject->getComponents(), out);

		out << YAML::EndMap;

		if (out.good())
			LOG_TRACE("Serialized game object: " + gameObject->name);
		else
			LOG_ERROR("Failed to serialize game object: " + gameObject->name);
	}

	// Serializes all components to the given YAML emitter, will create a sequence of components
	void GameSerializer::serializeComponents(std::vector<std::shared_ptr<Component>> components, YAML::Emitter& out)
	{
		out << YAML::Key << "Components";
		out << YAML::Value << YAML::BeginSeq;

		for (const auto& component : components)
		{
			serializeComponent(component, out);
		}

		out << YAML::EndSeq;
	}

	// Serializes a component to the given YAML emitter as a map
	void GameSerializer::serializeComponent(std::shared_ptr<Component> component, YAML::Emitter& out)
	{
		std::shared_ptr<Serializable> serializable = std::dynamic_pointer_cast<Serializable>(component);
		out << YAML::BeginMap;
		out << YAML::Key << "name";
		out << YAML::Value << component->getName();
		serializeSerializable(serializable.get(), out);
		out << YAML::EndMap;
	}

	// Serializes all textures to the given YAML emitter, will create a sequence of textures
	void GameSerializer::serializeTextures(const Game* game, YAML::Emitter& out)
	{

		out << YAML::Key << "Textures";
		out << YAML::Value << YAML::BeginSeq;
		for (const auto& [textureId, texture] : game->getTextures())
		{
			serializeTexture(texture.get(), out);
		}
		out << YAML::EndSeq;

		if (out.good())
			LOG_INFO("Serialized textures");
		else
			LOG_ERROR("Failed to serialize textures");
	}

	// Serializes a texture to the given YAML emitter as a map
	void GameSerializer::serializeTexture(Texture* texture, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		serializeSerializable(texture, out);
		out << YAML::EndMap;

		if (out.good())
			LOG_TRACE("Serialized texture: " + texture->name);
		else
			LOG_ERROR("Failed to serialize texture: " + texture->name);
	}

	// Serializes all materials to the given YAML emitter, will create a sequence of materials
	void GameSerializer::serializeMaterials(const Game* game, YAML::Emitter& out)
	{
		out << YAML::Key << "Materials";
		out << YAML::Value << YAML::BeginSeq;
		for (const auto& [materialId, material] : game->getMaterials())
		{
			serializeMaterial(material.get(), out);
		}

		out << YAML::EndSeq;

		if (out.good())
			LOG_INFO("Serialized materials");
		else
			LOG_ERROR("Failed to serialize materials");
	}

	// Serializes a material to the given YAML emitter as a map
	void GameSerializer::serializeMaterial(Material* material, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		serializeSerializable(material, out);
		out << YAML::EndMap;

		if (out.good())
			LOG_TRACE("Serialized material: " + material->name);
		else
			LOG_ERROR("Failed to serialize material: " + material->name);
	}

	void GameSerializer::serializeActions(const Game* game, YAML::Emitter& emitter)
	{
		ActionMap::getInstance().getActions()
	}

	// Serializes a serializable to the given YAML emitter as key value pairs for each serializable variable
	void GameSerializer::serializeSerializable(Serializable* serializable, YAML::Emitter& out)
	{
		for (const auto serializableVariable : serializable->getSerializableVariables())
		{
			out << YAML::Key << serializableVariable.name;
			if (serializableVariable.type == SerializableType::FLOAT)
			{
				out << YAML::Value << *static_cast<float*>(serializableVariable.data);
			}
			else if (serializableVariable.type == SerializableType::VECTOR3 || serializableVariable.type == SerializableType::COLOR)
			{
				float* vec3 = static_cast<float*>(serializableVariable.data);
				std::vector<float> vec3Vector(vec3, vec3 + 3);
				out << YAML::Flow << YAML::Value << vec3Vector;
			}
			else if (serializableVariable.type == SerializableType::VECTOR4)
			{
				float* vec4 = static_cast<float*>(serializableVariable.data);
				std::vector<float> vec4Vector(vec4, vec4 + 4);
				out << YAML::Flow << YAML::Value << vec4Vector;
			}
			else if (serializableVariable.type == SerializableType::BOOLEAN)
			{
				out << YAML::Value << *static_cast<bool*>(serializableVariable.data);
			}
			else if (serializableVariable.type == SerializableType::STRING)
			{
				out << YAML::Value << *static_cast<std::string*>(serializableVariable.data);
			}
			else if (serializableVariable.type == SerializableType::INT)
			{
				out << YAML::Value << *static_cast<int*>(serializableVariable.data);
			}
			else
			{
				LOG_ERROR("Failed to serialize serializable because of unknown serializable type");
			}
		}
	}

	// Deserializes the given serializable from the given YAML node
	// The given node should be the components node of a game object
	void GameSerializer::deserializeSerializable(YAML::Node node, Serializable* serializable)
	{
		// TODO: improve complexity, currently does unnecessary iterations
		for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
		{
			std::string nodeName = it->first.as<std::string>();
			for (auto serializableVariable : serializable->getSerializableVariables())
			{
				if (nodeName == serializableVariable.name)
				{
					if (serializableVariable.type == SerializableType::FLOAT)
					{
						*static_cast<float*>(serializableVariable.data) = it->second.as<float>();
					}
					else if (serializableVariable.type == SerializableType::VECTOR3 || serializableVariable.type == SerializableType::COLOR)
					{
						std::vector<float> vec3 = it->second.as<std::vector<float>>();
						std::copy(vec3.begin(), vec3.end(), static_cast<float*>(serializableVariable.data));
					}
					else if (serializableVariable.type == SerializableType::VECTOR4)
					{
						std::vector<float> vec4 = it->second.as<std::vector<float>>();
						std::copy(vec4.begin(), vec4.end(), static_cast<float*>(serializableVariable.data));
					}
					else if (serializableVariable.type == SerializableType::BOOLEAN)
					{
						*static_cast<bool*>(serializableVariable.data) = it->second.as<bool>();
					}
					else if (serializableVariable.type == SerializableType::STRING)
					{
						*static_cast<std::string*>(serializableVariable.data) = it->second.as<std::string>();
					}
					else if (serializableVariable.type == SerializableType::INT)
					{
						*static_cast<int*>(serializableVariable.data) = it->second.as<int>();
					}
					else
					{
						LOG_ERROR("Failed to deserialize serializable because of unknown serializable type");
					}

				}
			}
		}
	}

	// Deserializes the game config, state yaml files from the folder with the given game's name
	void GameSerializer::deserializeGame(Game* game)
	{
		LOG_INFO("Deserializing game: {}", game->name);
		std::string gameConfigFilePath = GAME_FOLDER_PATH + game->name + "/" + game->name + "Config" + GAME_CONFIG_FILE_EXTENSION;
		LOG_TRACE("Loading file: " + gameConfigFilePath);
		YAML::Node config = YAML::LoadFile(gameConfigFilePath);

		game->resetGameState();

		deserializeGameConfig(game);

		deserializeGameState(game);

		try {
			//	settings.useDarkTheme = config["useDarkMode"].as<bool>();
			//	settings.showGizmos = config["showGizmos"].as<bool>();
		}
		catch (const std::exception& e)
		{
			// If there is problems with deserialization, return default settings
			LOG_WARN("Failed to deserialize editor settings: " + std::string(e.what()));
		}


		LOG_INFO("Deserialized game: {}", game->name);
	}


	void GameSerializer::deserializeGameConfig(Game* game)
	{
	}

	// Deserializes the game state from the game name's folder
	void GameSerializer::deserializeGameState(Game* game)
	{
		LOG_INFO("Deserializing game state: {}", game->name);
		std::string gameStateFilePath = GAME_FOLDER_PATH + game->name + "/" + game->name + "State" + GAME_CONFIG_FILE_EXTENSION;
		LOG_TRACE("Loading file: " + gameStateFilePath);
		YAML::Node state = YAML::LoadFile(gameStateFilePath);


		deserializeTextures(state, game);
		deserializeMaterials(state, game);
		deserializeGameObjects(state, game);

		LOG_INFO("Deserialized game state: " + game->name);
	}

	// Deserializes all textures from the given YAML node into the game
	void GameSerializer::deserializeTextures(YAML::Node node, Game* game)
	{
		YAML::Node texturesNode;
		try
		{
			texturesNode = node["Textures"];
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Failed to deserialize textures: " + std::string(e.what()));
			return;
		}

		for (YAML::const_iterator it = texturesNode.begin(); it != texturesNode.end(); ++it)
		{
			try
			{
				YAML::Node textureNode = *it;
				std::string name = textureNode["name"].as<std::string>();
				std::string filename = textureNode["fileName"].as<std::string>();
				std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::create(filename));
				texture->uuid.id = textureNode["Id"].as<std::string>();
				game->addTexture(texture);
			}
			catch (const std::exception& e)
			{
				LOG_WARN("Failed to deserialize texture: " + std::string(e.what()));
			}

		}
	}

	// Deserializes all materials from the given YAML node into the game
	void GameSerializer::deserializeMaterials(YAML::Node node, Game* game)
	{
		YAML::Node materialNode;
		try
		{
			materialNode = node["Materials"];
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Failed to deserialize textures: " + std::string(e.what()));
			return;
		}

		for (YAML::const_iterator it = materialNode.begin(); it != materialNode.end(); ++it)
		{
			YAML::Node materialNode = *it;
			Material* material = new Material();

			deserializeSerializable(materialNode, material);

			std::string diffuseTextureId = materialNode["Diffuse Texture"].as<std::string>();
			std::string specularTextureId = materialNode["Specular Texture"].as<std::string>();

			if (diffuseTextureId != "")
			{
				material->diffuseTexture = game->getTexture(diffuseTextureId);
			}

			if (specularTextureId != "")
			{
				material->specularTexture = game->getTexture(specularTextureId);
			}

			game->addMaterial(std::shared_ptr<Material>(material));
		}
	}

	// Deserializes all game objects from the given YAML node into the game
	void GameSerializer::deserializeGameObjects(YAML::Node node, Game* game)
	{
		YAML::Node gameObjectsNode;
		try
		{
			gameObjectsNode = node["GameObjects"];
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Failed to deserialize game objects: " + std::string(e.what()));
			return;
		}

		for (YAML::const_iterator it = gameObjectsNode.begin(); it != gameObjectsNode.end(); ++it)
		{
			try
			{
				std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
				YAML::Node gameObjectNode = *it;
				gameObject->name = gameObjectNode["name"].as<std::string>();
				std::vector<float> transformMatrix = gameObjectNode["transform"].as<std::vector<float>>();
				std::copy(transformMatrix.begin(), transformMatrix.end(), &gameObject->transform.transformMatrix[0][0]);
				gameObject->isVisible = gameObjectNode["isVisible"].as<bool>();
				gameObject->uuid.id = gameObjectNode["Id"].as<std::string>();
				gameObject->tag = gameObjectNode["tag"].as<std::string>();
				deserializeComponents(gameObjectNode, gameObject.get(), game);
				game->addGameObject(gameObject);
			}
			catch (const std::exception& e)
			{
				LOG_WARN("Failed to deserialize game object: " + std::string(e.what()));
			}

		}

	}

	// Deserializes all components from the given YAML node into the given game object
	void GameSerializer::deserializeComponents(YAML::Node node, GameObject* gameObject, Game* game)
	{
		YAML::Node componentsNode;
		try
		{
			componentsNode = node["Components"];
		}
		catch (const std::exception& e)
		{
			LOG_WARN("Failed to deserialize components: " + std::string(e.what()));
			return;
		}

		for (YAML::const_iterator it = componentsNode.begin(); it != componentsNode.end(); ++it)
		{
			try
			{
				YAML::Node componentNode = *it;
				deserializeComponent(componentNode, gameObject, game);
			}
			catch (const std::exception& e)
			{
				LOG_WARN("Failed to deserialize component: " + std::string(e.what()));
			}
		}
	}

	// Deserializes a component from the given YAML node into the given game object
	void GameSerializer::deserializeComponent(YAML::Node node, GameObject* gameObject, Game* game)
	{
		try
		{
			std::string componentName = node["name"].as<std::string>();

			std::shared_ptr<Component> component = ComponentFactory::createComponent(componentName);

			if (component == nullptr)
			{
				LOG_WARN("Failed to deserialize component: " + componentName + " because it is nullptr");
				return;
			}

			deserializeSerializable(node, component.get());

			// Special case for mesh component
			if (componentName == "Mesh")
			{
				MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(component.get());
				if (meshComponent != nullptr)
				{
					if (meshComponent->objFileName != "")
					{
						MeshComponent::loadMeshFromOBJFile(meshComponent->objFileName, meshComponent);
					}

					if (meshComponent->primativeTypeAsString != "" && meshComponent->primativeTypeAsString != "NONE")
					{
						meshComponent->primativeType = MeshComponent::stringToPrimativeType(meshComponent->primativeTypeAsString);
						MeshComponent::loadPrimativeMesh(meshComponent->primativeType, meshComponent);
					}

					std::string materialId = node["Material"].as<std::string>();
					if (materialId != "")
					{
						meshComponent->setMaterial(game->getMaterial(materialId));
					}
				}
			}

			if (auto scriptableComponent = dynamic_cast<ScriptableComponent*>(component.get()))
			{
				// We need to initialize the lua state for the scriptable component
				ScriptEngine::getInstance()->initializeLuaStateForScriptableComponent(scriptableComponent);
			}

			gameObject->addComponent(component);

		}
		catch (const std::exception& e)
		{
			LOG_WARN("Failed to deserialize component: " + std::string(e.what()));
		}
	}
}
