#include "EditorSerializer.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>
#include "Logger.hpp"
#include <filesystem>
#include "GameSerializer.hpp"
namespace fs = std::filesystem;


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
		const std::string fullPath = filePath + fileName + EDITOR_CONFIG_FILE_EXTENSION;
		LOG_INFO("Creating YAML file: " + fullPath);
		std::ofstream outfile(fullPath);
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

		LOG_INFO("Serialized editor settings: ");
	}
	EditorSettings EditorSerializer::deSerializeEditorSettings()
	{
		LOG_INFO("Deserializing editor settings: ");

		std::string fileName = EDITOR_CONFIG_FILE_NAME;
		
		YAML::Node config = YAML::LoadFile(EDITOR_FOLDER_PATH + fileName + EDITOR_CONFIG_FILE_EXTENSION);

		EditorSettings settings{};

		try {
			settings.useDarkTheme = config["useDarkMode"].as<bool>();
			settings.showGizmos = config["showGizmos"].as<bool>();
		}
		catch(const std::exception& e)
		{
			// If there is problems with deserialization, return default settings
			LOG_WARN("Failed to deserialize editor settings: " + std::string(e.what()));
			return EditorSettings();
		}
		
		LOG_INFO("Deserialized editor settings: ");
		return settings;
	}
	std::vector<std::string> EditorSerializer::getAllGameNamesInGamesFolder()
	{
		std::string path = GAME_FOLDER_PATH;
		std::vector<std::string> gameNames{};

		for (const auto& entry : fs::directory_iterator(path))
			gameNames.push_back(entry.path().filename().string());
			

		return gameNames;
	}
}
