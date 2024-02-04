#pragma once
#include "GraphicsAPI.hpp"
#include <memory>
#include "Shader.hpp"
#include <vector>
#include "Core/Game.hpp"
#include "RendererSettings.hpp"

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame(Game* game, CameraComponent* camera, RendererSettings* renderingSettings);

		static void drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, Camera* camera);

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();

		static std::unique_ptr<Shader> baseShader;
	};
}