#pragma once
#include "GraphicsAPI.hpp"
#include <memory>
#include "GameObject.hpp"
#include "Shader.hpp"

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame(GameObject& gameObject);

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();

		static std::unique_ptr<Shader> baseShader;
	};
}