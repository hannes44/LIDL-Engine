#pragma once

#include <string>
#include "GraphicsAPI.hpp"

namespace engine
{
	struct GameConfig
	{
		std::string gameName = "Default Game";

		bool isDefaultFullscreen = false;

		GraphicsAPIType graphicsAPIType = GraphicsAPIType::OpenGL;
	};
}