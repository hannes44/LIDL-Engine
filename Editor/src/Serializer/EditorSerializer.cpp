#include "EditorSerializer.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>
#include <Engine.hpp>
#include <filesystem>
#include "Serializer/GameSerializer.hpp"


namespace fs = std::filesystem;


namespace engine
{
#define EDITOR_CONFIG_FILE_NAME "editor_settings"

	EditorSerializer::EditorSerializer()
	{
	}
	void EditorSerializer::createYAMLFile(const std::string& filePath, const std::string& fileName)
	{
		const std::string fullPath = filePath + fileName + CONFIG_FILE_EXTENSION;
		LOG_INFO("Creating YAML file: " + fullPath);
		std::ofstream outfile(fullPath);
		outfile.close();
	}
	void EditorSerializer::serializeEditorSettings(EditorSettings& settings)
	{
		LOG_INFO("Serializing editor settings: ");
		createYAMLFile(PATH_TO_EDITOR_FOLDER, EDITOR_CONFIG_FILE_NAME);

		YAML::Emitter out;
		out << YAML::BeginMap;

		GameSerializer::serializeSerializable(&settings, out);

		GameSerializer::serializeSerializable(&settings.rendererSettings, out);

		out << YAML::EndMap;

		std::string fileName = EDITOR_CONFIG_FILE_NAME;
		std::ofstream fout(PATH_TO_EDITOR_FOLDER + fileName + CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		LOG_INFO("Serialized editor settings: ");
	}
	EditorSettings EditorSerializer::deSerializeEditorSettings()
	{
		LOG_INFO("Deserializing editor settings: ");


		std::string fileName = EDITOR_CONFIG_FILE_NAME;

		YAML::Node config = YAML::LoadFile(ResourceManager::getPathToEditorResource(fileName + CONFIG_FILE_EXTENSION));

		EditorSettings settings{};

		GameSerializer::deserializeSerializable(config, &settings);

		GameSerializer::deserializeSerializable(config, &settings.rendererSettings);

		LOG_INFO("Deserialized editor settings: ");
		return settings;
	}

}
