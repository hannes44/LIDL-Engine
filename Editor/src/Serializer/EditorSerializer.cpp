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
	#define EDITOR_CONFIG_FILE_NAME "editor_settings"
	#define GAME_ASSETS_FOLDER "../../../assets/3DObjects/"
	#define WIN32_API_ERROR_CODE_FILE_ALREADY_EXISTS 80

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

	// copies file from file explorer to subdirectory and returns the file name
	// if no file is selected, returns empty string
	std::string EditorSerializer::addFileFromWindowsExplorerToProject(Project* project, char* fileExplorerFilter)
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

		std::string destinationPath = fs::current_path().string() + GAME_ASSETS_FOLDER + objFileName;
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
			bool sameFileExists = fs::file_size(fs::current_path().string() + GAME_ASSETS_FOLDER + objFileName) == objFileSize;
			if (sameFileExists)
			{
				std::string newDestinationPath = fs::current_path().string() + GAME_ASSETS_FOLDER + objFileName;
				LOG_INFO("File: {} already exists in assets: {}", objFileName, newDestinationPath);
				return objFileName;
			}
			
			LOG_TRACE("File name already exists! Trying again with number extension");

			// It will not recheck the size to determine if the file is the same after the initial check, not likely to happen but could fix it later
			for (int i = 1; i < 100; i++)
			{
				CopyFile(ofn.lpstrFile, (fs::current_path().string() + GAME_ASSETS_FOLDER + objFileNameNoExtension + std::to_string(i) + objFileExtension).c_str(), TRUE);

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
}
