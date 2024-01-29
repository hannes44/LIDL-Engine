#include "EditorSerializer.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>
#include "Logger.hpp"

namespace engine
{
	#define EDITOR_FOLDER_PATH "../../editor/"
	#define EDITOR_CONFIG_FILE_EXTENSION ".yaml"
	#define EDITOR_CONFIG_FILE_NAME "editor_settings"

	EditorSerializer::EditorSerializer()
	{
	}
	void EditorSerializer::createYAMLFile(const std::string& filePath, const std::string& fileName)
	{
		LOG_INFO("Creating YAML file: " + filePath + fileName + EDITOR_CONFIG_FILE_EXTENSION);
		std::ofstream outfile(filePath + fileName + EDITOR_CONFIG_FILE_EXTENSION);
		outfile.close();
	}
	void EditorSerializer::serializeEditorSettings(EditorSettings& settings)
	{
		LOG_INFO("Serializing editor settings: ");
		createYAMLFile(EDITOR_FOLDER_PATH, EDITOR_CONFIG_FILE_NAME);

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "useDarkMode";
		out << YAML::Value << settings.useDarkTheme;

		out << YAML::Key << "showGizmos";
		out << YAML::Value << settings.showGizmos;

		out << YAML::EndMap;

		std::string fileName = EDITOR_CONFIG_FILE_NAME;
		std::ofstream fout(EDITOR_FOLDER_PATH + fileName + EDITOR_CONFIG_FILE_EXTENSION);
		fout << out.c_str();

		std::cout << "Here's the output YAML:\n" << out.c_str();

		LOG_INFO("Serialized editor settings: ");
	}
	EditorSettings EditorSerializer::deSerializeEditorSettings()
	{
		LOG_INFO("Deserializing editor settings: ");

		std::string fileName = EDITOR_CONFIG_FILE_NAME;
		//std::ifstream fin(EDITOR_FOLDER_PATH + fileName + EDITOR_CONFIG_FILE_EXTENSION);
		
		YAML::Node config = YAML::LoadFile(EDITOR_FOLDER_PATH + fileName + EDITOR_CONFIG_FILE_EXTENSION);

		EditorSettings settings{};

		try {
			settings.useDarkTheme = config["useDarkMode"].as<bool>();
			settings.showGizmos = config["showGizmos"].as<bool>();
		}
		catch(const std::exception& e)
		{
			// Id there is problems with deserialization, return default settings
			LOG_WARN("Failed to deserialize editor settings: " + std::string(e.what()));
			return EditorSettings();
		}
		
		LOG_INFO("Deserialized editor settings: ");
		return settings;
	}
}
