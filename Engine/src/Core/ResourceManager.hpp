#pragma once
#include <string>
#include "Core/Game.hpp"

namespace engine
{
#define CONFIG_FILE_EXTENSION ".yaml"
#define PATH_TO_EDITOR_FOLDER "../../Editor/"
#define PATH_TO_GAMES_FOLDER "../../Games/" // This is only for the editor to find the games

	enum class ResourceType
	{
		TEXTURE,
		OBJECT3D,
		SOUND,
		SCRIPT,
		CONFIG,
		OTHER
	};

	class ResourceManager
	{
	public:
		// Non cloneable
		ResourceManager(const ResourceManager& other) = delete;

		// Non copyable
		void operator=(const ResourceManager& other) = delete;

		// This function will change the game that the ResourceManager will look for resources in
		void changeGame(Game* game);

		std::string getPathToGameResource(const std::string& fileName);

		static std::string getPathToEditorResource(const std::string& fileName);

		static std::string getAbsolutePathToEditorGamesFolder();

		std::string getPathToActiveGameFolder();

		std::string getPathToActiveGameSpecificAssetFolder(ResourceType type);

		std::string getPathToActiveGameAsset3DObjectsFolder();

		std::vector<std::string> getAllCSharpScriptsInActiveGame();

		void createNewScriptForActiveGame(const std::string& scriptFileName);

		static std::vector<std::string> getAllGameNamesInGamesFolder();

		static bool isProjectPathValid(const std::string& path);

		static bool isProjectNameValid(const std::string& name);

		static void createFolder(const std::string& path);

		static std::string addFileFromWindowsExplorerToProject(char* fileExplorerFilter);

		static std::string getFolderPathFromFileExplorer(const std::string initialPath = "");

		static std::string getResourceFolderName(ResourceType type);

		// Will map the given file name to a resource type based on the file extension
		static ResourceType getResourceTypeFromFileName(const std::string& fileName);

		static ResourceManager* getInstance()
		{
			if (instance == nullptr)
			{
				instance = new ResourceManager();
			}
			return instance;
		}

		// This is public so that the game dll can get its state
		// TODO: this should be private
		inline static ResourceManager* instance;
	private:
		Game* game = nullptr;

		ResourceManager() {};
	};
}