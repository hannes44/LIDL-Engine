#pragma once

#include <string>
#include "Renderer/GraphicsAPI.hpp"
#include "Physics/GamePhysicsSettings.hpp"

namespace engine
{
	struct GameConfig
	{
		std::string gameName = "Default Game";

		bool isDefaultFullscreen = false;

		GraphicsAPIType graphicsAPIType = GraphicsAPIType::OpenGL;
		GamePhysicsSettings physicsSettings{};
	};
}