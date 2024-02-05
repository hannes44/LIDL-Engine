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
		void renderGame(Game* game, CameraComponent* camera, RendererSettings* renderingSettings);

		void renderGizmos(Game* game, CameraComponent* camera, RendererSettings* renderingSettings);

		void drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, CameraComponent* camera);

		void initGraphicsAPI(GraphicsAPIType type);

		std::unique_ptr<GraphicsAPI> graphicsAPI = nullptr;

		GraphicsAPIType getAPIType();

		std::unique_ptr<Shader> baseShader = nullptr;

		static Renderer* getInstance()
		{
			if (instance == nullptr)
			{
				instance = new Renderer();
			}
			return instance;
		}

		// Temporary hack to sync the renderer state with the editor and game
		inline static Renderer* instance;
	};
}