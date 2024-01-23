#include "Renderer.hpp"
#include "Logger.hpp"

namespace engine
{
	void engine::Renderer::renderGame()
	{
	}
	void Renderer::initGraphicsAPI(GraphicsAPIType type)
	{
	}
	GraphicsAPIType Renderer::getAPIType()
	{
		if (graphicsAPI == nullptr)
		{
			LOG_FATAL("Graphics API is not initialized!");
			abort();
		}
			
		return graphicsAPI->getType();
	}

	std::unique_ptr<GraphicsAPI> Renderer::graphicsAPI = nullptr;
}
