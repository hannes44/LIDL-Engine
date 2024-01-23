#pragma once
#include "GraphicsAPI.hpp"
#include <memory>
#include "GameObject.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

namespace engine
{
	class Renderer
	{
	public:
		static void renderGame(GameObject& gameObject, Camera& camera);

		static void initGraphicsAPI(GraphicsAPIType type);

		static std::unique_ptr<GraphicsAPI> graphicsAPI;

		static GraphicsAPIType getAPIType();

		static std::unique_ptr<Shader> baseShader;
	};
}