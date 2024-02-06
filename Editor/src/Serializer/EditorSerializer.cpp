#include "EditorSerializer.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>
#include <Engine.hpp>
#include <filesystem>
#include "Serializer/GameSerializer.hpp"
#include <Windows.h>
#include <ShlObj.h>
#include <commdlg.h>
#include <filesystem>
#include <ShObjIdl_core.h>

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

		GameSerializer::serializeSerializable(&settings, out);

		GameSerializer::serializeSerializable(&settings.rendererSettings, out);

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

		GameSerializer::deserializeSerializable(config, &settings);

		GameSerializer::deserializeSerializable(config, &settings.rendererSettings);
		
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

	bool EditorSerializer::isProjectPathValid(const std::string& path)
	{
		if (path.empty())
			return false;

		return true;
	}

	bool EditorSerializer::isProjectNameValid(const std::string& name)
	{
		if (name.empty())
			return false;

		return true;
	}

	void EditorSerializer::createFolder(const std::string& path)
	{
		LOG_INFO("Creating folder: " + path);
		CreateDirectory(path.c_str(), NULL);
		LOG_INFO("Created folder: " + path);
	}

	// Gets the path to the folder that the user selected in the file explorer (Windows only)
	// This function is blocking until the user selects a folder
	std::string EditorSerializer::getFolderPathFromFileExplorer(const std::string initialPath)
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
	std::string EditorSerializer::getPathToEditorGamesFolder()
	{
		char path[100];
		GetModuleFileNameA(NULL, path, 100);
		std::string pathToEditorFolder = path;
		// Turn the slashes around
		std::replace(pathToEditorFolder.begin(), pathToEditorFolder.end(), '\\', '/');

		// Unlucky
		pathToEditorFolder = pathToEditorFolder.substr(0, pathToEditorFolder.find_last_of("\\/"));
		pathToEditorFolder = pathToEditorFolder.substr(0, pathToEditorFolder.find_last_of("\\/"));
		pathToEditorFolder = pathToEditorFolder.substr(0, pathToEditorFolder.find_last_of("\\/"));
		return pathToEditorFolder + "/editor/games/";
	}
}
