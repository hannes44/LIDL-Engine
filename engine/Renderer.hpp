#pragma once
#include "GraphicsAPI.hpp"
#include <memory>

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame();

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();
	};
}