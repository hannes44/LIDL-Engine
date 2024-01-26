#pragma once
#include "GraphicsAPI.hpp"
#include <memory>
#include "Shader.hpp"
#include <vector>
#include "Game.hpp"

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame(Game* game, Camera& camera);

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();

		static std::unique_ptr<Shader> baseShader;
	};
}