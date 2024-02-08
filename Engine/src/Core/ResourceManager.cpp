#include "ResourceManager.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include "Core/Logger.hpp"
#include <Windows.h>

namespace fs = std::filesystem;

namespace engine
{
	std::string ResourceManager::getPathToGameResource(const std::string& fileNameWithExtention, Game* game)
	{
		std::string pathToSearch = PATH_TO_GAMES_FOLDER;

		for (const auto& entry : fs::directory_iterator(pathToSearch))
		{
			std::cout << entry.path() << std::endl;
		}

		// Since finding resources isn't a critical part of the engine in regards to performance, we can afford to 
		// bruteforce the search for the file. If performance becomes an issue, we can start searching the correct folders depending on the file extension

		return "";
	}

	// This function will return the path to the given filename in the editor's folders
	std::string ResourceManager::getPathToEditorResource(const std::string& fileNameWithExtention)
	{
		std::string pathToEditor = PATH_TO_EDITOR_FOLDER;
		std::string fileNameExtension = fileNameWithExtention.substr(fileNameWithExtention.find_last_of("."));
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
			
			if (currentFileName == fileNameWithExtention)
			{
				std::string currentFilePath = entry.path().string();
				LOG_INFO("Found file: {}, at path: {}", currentFileName, currentFilePath);
				return currentFilePath;
			}
		}

		LOG_WARN("Could not find file: " + fileNameWithExtention);
		return "";
	}

	std::string ResourceManager::getAbsolutePathToEditorGamesFolder()
	{
		char path[100];
		// Gettinsg the absolute path to the build folder
		GetModuleFileNameA(NULL, path, 100);
		std::string pathToEditorFolder = path;
		// Turn the slashes around
		std::replace(pathToEditorFolder.begin(), pathToEditorFolder.end(), '\\', '/');

		// Remove the last part of the path to turn it into the path to the root of the project
		pathToEditorFolder = pathToEditorFolder.substr(0, pathToEditorFolder.find_last_of("\\/"));
		pathToEditorFolder = pathToEditorFolder.substr(0, pathToEditorFolder.find_last_of("\\/"));
		pathToEditorFolder = pathToEditorFolder.substr(0, pathToEditorFolder.find_last_of("\\/"));

		return pathToEditorFolder + "/editor/games/";
	}
}
