#pragma once
#include <string>
#include "Game.hpp"

namespace engine
{
	#define CONFIG_FILE_EXTENSION ".yaml"
	#define PATH_TO_EDITOR_FOLDER "../../Editor/"

#ifdef EDITOR
	#define PATH_TO_GAMES_FOLDER "../../Games/"
#else
	#define PATH_TO_GAMES_FOLDER "../../../Games/"
#endif

	class ResourceManager
	{
	public:
		static std::string getPathToGameResource(const std::string& fileName, Game* game);

		static std::string getPathToEditorResource(const std::string& fileName);

		static std::string getAbsolutePathToEditorGamesFolder();

	};
}