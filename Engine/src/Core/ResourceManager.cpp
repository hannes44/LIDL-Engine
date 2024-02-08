#include "ResourceManager.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include "Core/Logger.hpp"
#include <Windows.h>

namespace fs = std::filesystem;

namespace engine
{
	void ResourceManager::changeGame(Game* game)
	{
		this->game = game;
	}
	std::string ResourceManager::getPathToGameResource(const std::string& fileNameWithExtention)
	{
		LOG_INFO("Looking for file: {}", fileNameWithExtention);
		if (game == nullptr)
		{
			LOG_ERROR("No game has been set for the ResourceManager");
			return "";
		}
		std::string pathToSearch = getPathToActiveGameFolder() + "Assets/";

		// Since finding resources isn't a critical part of the engine in regards to performance, we can afford to 
		// bruteforce the search for the file. If performance becomes an issue, we can start searching the correct folders depending on the file extension
		for (const auto& entry : fs::directory_iterator(pathToSearch))
		{
			if (!entry.is_directory())
				continue;

			for (const auto& subFolderEntry : fs::directory_iterator(entry.path()))
			{
				if (subFolderEntry.path().filename().string() == fileNameWithExtention)
				{
					LOG_INFO("Found file: {}", subFolderEntry.path().string());
					return subFolderEntry.path().string();
				}
			}
		}
		LOG_ERROR("Could not find file: " + fileNameWithExtention);
		return "";
	}

	

	// This function will return the path to the given filename in the editor's folders
	std::string ResourceManager::getPathToEditorResource(const std::string& fileNameWithExtention)
	{
		LOG_INFO("Looking for file: {}", fileNameWithExtention);
		std::string pathToEditor = PATH_TO_EDITOR_FOLDER;
		std::string fileNameExtension = fileNameWithExtention.substr(fileNameWithExtention.find_last_of("."));

		std::string pathToSearch = pathToEditor + "Assets/";

		// If the file is a config file, we will search in the editor's folder
		if (fileNameExtension == CONFIG_FILE_EXTENSION)
		{
			pathToSearch = pathToEditor;
		}

		// Since finding resources isn't a critical part of the editor in regards to performance, we can afford to 
		// bruteforce the search for the file. If performance becomes an issue, we can start searching the correct folders depending on the file extension
		for (const auto& entry : fs::directory_iterator(pathToSearch))
		{
			// If the file is not inside a subfolder
			if (entry.path().filename().string() == fileNameWithExtention)
			{
				LOG_INFO("Found file: {}", entry.path().string());
				return entry.path().string();
			}

			if (!entry.is_directory())
				continue;

			// Check one layer deep in all subfolders
			for (const auto& subFolderEntry : fs::directory_iterator(entry.path()))
			{
				if (subFolderEntry.path().filename().string() == fileNameWithExtention)
				{
					LOG_INFO("Found file: {}", subFolderEntry.path().string());
					return subFolderEntry.path().string();
				}
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

	std::string ResourceManager::getPathToActiveGameFolder()
	{
		std::string pathToSearch = PATH_TO_GAMES_FOLDER;

		// TODO: This should be done with macros. PATH_TO_GAMES_FOLDER should be set to the correct path depending on if games or editor is building
		if (std::filesystem::current_path().filename() != "Editor")
		{
			pathToSearch = "../" + pathToSearch;
		}

		pathToSearch += game->name + "/";

		return pathToSearch;
	}
	std::string ResourceManager::getPathToActiveGameAsset3DObjectsFolder()
	{
		return getPathToActiveGameFolder() + "Assets/3DObjects/";
	}
}
