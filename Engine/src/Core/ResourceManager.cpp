#include "ResourceManager.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include "Core/Logger.hpp"
#define _WINSOCKAPI_
#include <Windows.h>
#include <ShlObj.h>
#include <commdlg.h>
#include <filesystem>
#include <ShObjIdl_core.h>
#include "ScriptEngine/ScriptEngine.hpp"

#include <fstream>
#include <sstream>
#include <regex>


namespace fs = std::filesystem;

namespace engine
{
#define WIN32_API_ERROR_CODE_FILE_ALREADY_EXISTS 80

	void ResourceManager::changeGame(Game* game)
	{
		this->game = game;
	}
	std::string ResourceManager::getPathToGameResource(const std::string& fileNameWithExtention)
	{
		LOG_TRACE("Looking for file: {}", fileNameWithExtention);
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
					LOG_TRACE("Found file: {}", subFolderEntry.path().string());
					return subFolderEntry.path().string();
				}
			}
		}
			
		// We also need to search the scripts folder
		pathToSearch = getPathToActiveGameFolder() + "Scripts/";

		for (const auto& entry : fs::directory_iterator(pathToSearch))
		{
			if (entry.path().filename().string() == fileNameWithExtention)
			{
				LOG_TRACE("Found file: {}", entry.path().string());
				return entry.path().string();
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

		// If the file is a config file or a script file, we will search in the editor's folder
		if (fileNameExtension == CONFIG_FILE_EXTENSION || fileNameExtension == ".cs" || fileNameExtension == ".lua")
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

	// Returns the path to the asset folder with the given type
	std::string ResourceManager::getPathToActiveGameSpecificAssetFolder(ResourceType type)
	{
		std::string pathToGameAssetFolder = getPathToActiveGameFolder() + "Assets/";

		std::string pathToSpecificAssetFolder = pathToGameAssetFolder + getResourceFolderName(type) + "/";
		return pathToSpecificAssetFolder;
	}
	std::string ResourceManager::getPathToActiveGameAsset3DObjectsFolder()
	{
		return getPathToActiveGameFolder() + "Assets/3DObjects/";
	}

	std::vector<std::string> ResourceManager::getAllCSharpScriptsInActiveGame()
	{
		std::string path = getPathToActiveGameFolder() + "Scripts/";
		std::vector<std::string> scriptNames{};
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (entry.path().extension() == ".cs")
				scriptNames.push_back(entry.path().filename().string());
		}
			
		return scriptNames;
	}

	void ResourceManager::createNewScriptForActiveGame(const std::string& scriptFileName)
	{
		// Copying the template script to the active game's script folder
		std::string sourcePath = getPathToEditorResource("ComponentTemplate.cs");
		std::string destinationPath = getPathToActiveGameFolder() + "Scripts/" + scriptFileName;
		CopyFile(sourcePath.c_str(), destinationPath.c_str(), TRUE);

		// Modifying the script to have the correct class name
		std::ostringstream text;
		std::ifstream in_file(destinationPath);
		text << in_file.rdbuf();
		std::string str = text.str();

		std::string className = scriptFileName.substr(0, scriptFileName.find_last_of("."));
		std::string oldClassName = "TemplateComponent";
		
		std::string const result = std::regex_replace(str, std::regex(oldClassName), className);

		in_file.close();

		std::ofstream out_file(destinationPath);
		out_file << result;
	}
	
	std::vector<std::string> ResourceManager::getAllGameNamesInGamesFolder()
	{
		std::string path = "../../games/";
		std::vector<std::string> gameNames{};

		for (const auto& entry : fs::directory_iterator(path))
			gameNames.push_back(entry.path().filename().string());


		return gameNames;
	}

	bool ResourceManager::isProjectPathValid(const std::string& path)
	{
		if (path.empty())
			return false;

		return true;
	}

	bool ResourceManager::isProjectNameValid(const std::string& name)
	{
		if (name.empty())
			return false;

		return true;
	}

	void ResourceManager::createFolder(const std::string& path)
	{
		LOG_INFO("Creating folder: " + path);
		CreateDirectory(path.c_str(), NULL);
		LOG_INFO("Created folder: " + path);
	}

	// copies file from file explorer to subdirectory and returns the file name
	// if no file is selected, returns empty string
	std::string ResourceManager::addFileFromWindowsExplorerToProject(char* fileExplorerFilter)
	{
		OPENFILENAME ofn = { 0 };
		TCHAR szFile[260] = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = fileExplorerFilter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = fs::current_path().string().c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileName(&ofn) == FALSE)
		{
			LOG_INFO("User Cancelled File Explorer Dialog");
			return "";
		}

		// use ofn.lpstrFile here
		std::cout << ofn.lpstrFile << std::endl;
		fs::path objFilePath = ofn.lpstrFile;
		std::string objFileName = objFilePath.filename().string();
		std::string objFileNameNoExtension = objFilePath.stem().string();
		std::string objFileExtension = objFilePath.extension().string();
		int objFileSize = fs::file_size(objFilePath);

		ResourceType type = getResourceTypeFromFileName(objFileName);
		std::string destinationFolderName = getResourceFolderName(type);

		std::string destinationPath = ResourceManager::getInstance()->getPathToActiveGameSpecificAssetFolder(type) + objFileName;
		LOG_INFO("Copying file from file explorer to assets: " + destinationPath);
		CopyFile(ofn.lpstrFile, (destinationPath).c_str(), TRUE);

		// If the file was copied successfully
		if (GetLastError() == 0)
		{
			LOG_INFO("Copied file: {} from file explorer to assets: {}", objFileName, destinationPath);

			return objFileName;
		}

		// If the file already exists
		if (GetLastError() == WIN32_API_ERROR_CODE_FILE_ALREADY_EXISTS)
		{
			// Assuming the file is the same one if the size is the same
			bool sameFileExists = fs::file_size(destinationPath) == objFileSize;
			if (sameFileExists)
			{
				std::string newDestinationPath = fs::current_path().string() + destinationPath + objFileName;
				LOG_INFO("File: {} already exists in assets: {}", objFileName, newDestinationPath);
				return objFileName;
			}

			LOG_TRACE("File name already exists! Trying again with number extension");

			// It will not recheck the size to determine if the file is the same after the initial check, not likely to happen but could fix it later
			for (int i = 1; i < 100; i++)
			{
				CopyFile(ofn.lpstrFile, (fs::current_path().string() + destinationPath + objFileNameNoExtension + std::to_string(i) + objFileExtension).c_str(), TRUE);

				objFileName = objFileNameNoExtension + std::to_string(i) + objFileExtension;

				if (GetLastError() != WIN32_API_ERROR_CODE_FILE_ALREADY_EXISTS)
				{
					break;
				}
			}
		}

		// Unknown error
		LOG_ERROR("Error copying file from file explorer to assets: " + destinationPath);
		return "";
	}

	// Gets the path to the folder that the user selected in the file explorer (Windows only)
	// This function is blocking until the user selects a folder
	std::string ResourceManager::getFolderPathFromFileExplorer(const std::string initialPath)
	{
		const int bufferSize = 100;
		char folderPath[bufferSize];

		bool success = false;

		IFileDialog* pfd;
		// If the fileOpenDialog object is created successfully
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
		{
			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
			{
				pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
			}

			IShellItem* psi;

			// TODO: Use initialPath as the default folder
			GUID folderId = FOLDERID_RecycleBinFolder;

			SHGetKnownFolderItem(folderId,
				KF_FLAG_DEFAULT, nullptr, IID_PPV_ARGS(&psi));

			pfd->SetFolder(psi);
			psi->Release();

			// If the explorer window is opened successfully
			if (SUCCEEDED(pfd->Show(NULL)))
			{
				IShellItem* psi;
				if (SUCCEEDED(pfd->GetResult(&psi)))
				{
					PWSTR pszFilePath;
					if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath)))
					{
						MessageBox(NULL, "GetIDListName() failed", NULL, NULL);
					}

					// Convert the PWSTR to char*
					wcstombs(folderPath, pszFilePath, bufferSize);
					success = true;
					psi->Release();
				}
			}
			pfd->Release();
		}

		return success ? folderPath : "";
	}

	// Returns the folder name of the given resource type in the format "ExampleFolderName"
	std::string ResourceManager::getResourceFolderName(ResourceType type)
	{
		switch (type)
		{
		case ResourceType::TEXTURE:
			return "Textures";
		case ResourceType::OBJECT3D:
			return "3DObjects";
		case ResourceType::SOUND:
			return "Sounds";
		case ResourceType::SCRIPT:
			return "Scripts";
		case ResourceType::CONFIG:
			return "Configs";

			return std::string();
		}
	}

	ResourceType ResourceManager::getResourceTypeFromFileName(const std::string& fileName)
	{
		std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
		if (extension == "png" || extension == "PNG" || extension == "jpg" || extension == "jpeg")
			return ResourceType::TEXTURE;
		else if (extension == "obj")
			return ResourceType::OBJECT3D;
		else if (extension == "wav")
			return ResourceType::SOUND;
		else if (extension == "yaml")
			return ResourceType::CONFIG;

		LOG_INFO("Unknown file extension: " + extension);
		return ResourceType::OTHER;
	}
}