#pragma once
#include "GraphicsAPI.hpp"
#include <memory>
#include "GameObject.hpp"

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame(GameObject& gameObject);

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();
	};
}