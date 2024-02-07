#include "ResourceManager.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include "Core/Logger.hpp"

namespace fs = std::filesystem;

namespace engine
{
	#define PATH_TO_EDITOR "../../Editor/"
	#define CONFIG_FILE_EXTENSION "yaml"


	std::string ResourceManager::getPathToGameResource(const std::string& fileName, Game* game)
	{

		// Since finding resources isn't a critical part of the engine in regards to performance, we can afford to 
		// bruteforce the search for the file. If performance becomes an issue, we can start searching the correct folders depending on the file extension

		return "";
	}

	// This function will return the path to the given filename in the editor's folders
	std::string ResourceManager::getPathToEditorResource(const std::string& fileName)
	{
		std::string pathToEditor = PATH_TO_EDITOR;
		std::string fileNameExtension = fileName.substr(fileName.find_last_of(".") + 1);
		LOG_INFO("{}", fileNameExtension);

		std::string pathToAssetFolder = pathToEditor + "assets/";

		std::string pathToTextures = pathToAssetFolder + "Textures/";

		std::vector<std::string> textureFileNames{};

		std::string pathToSearch = pathToTextures;

		// If the file is a config file, we will search in the editor's folder
		if (fileNameExtension == CONFIG_FILE_EXTENSION)
		{
			pathToSearch = pathToEditor;
		}

		for (const auto& entry : fs::directory_iterator(pathToSearch))
		{
			std::string currentFileName = entry.path().filename().string();
			
			if (currentFileName == fileName)
			{
				std::string currentFilePath = entry.path().string();
				LOG_INFO("Found file: {}, at path: {}", currentFileName, currentFilePath);
				return currentFilePath;
			}
		}

		LOG_WARN("Could not find file: " + fileName);
		return "";
	}
}
