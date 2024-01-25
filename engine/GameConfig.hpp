#pragma once

#include <string>
#include "Renderer.hpp"

namespace engine
{
	struct GameConfig
	{
		std::string gameName = "Default Game";

		bool isDefaultFullscreen = false;

		GraphicsAPIType graphicsAPIType = GraphicsAPIType::OpenGL;
	};
}