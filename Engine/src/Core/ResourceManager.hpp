#pragma once
#include <string>
#include "Game.hpp"

namespace engine
{
	class ResourceManager
	{
	public:
		static std::string getPathToGameResource(const std::string& fileName, Game* game);

		static std::string getPathToEditorResource(const std::string& fileName);
	};
}