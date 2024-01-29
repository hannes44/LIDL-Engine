#pragma once
#include "GraphicsAPI.hpp"
#include <memory>
#include "Shader.hpp"
#include <vector>
#include "Game.hpp"
#include "RendererSettings.hpp"

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame(Game* game, Camera* camera, RendererSettings* renderingSettings);

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();

		static std::unique_ptr<Shader> baseShader;
	};
}