#include "Renderer.hpp"
#include "Logger.hpp"
#include "OpenGLGraphicsAPI.hpp"
#include "GameObject.hpp"
#include "MeshComponent.hpp"

namespace engine
{
	void Renderer::renderGame(GameObject& gameObject)
	{
		graphicsAPI->setClearColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		// TODO: Only gameobjects with a mesh should be sent to the renderer to aviod looping
		for (auto component : gameObject.components)
		{
			if (dynamic_cast<MeshComponent*>(component.get()))
			{
				LOG_INFO("Found mesh component");
			}
		}

		//graphicsAPI->drawIndexed(gameObject., gameObject.getIndexCount());
	}
	void Renderer::initGraphicsAPI(GraphicsAPIType type)
	{
		switch (type)
		{
		case GraphicsAPIType::OpenGL:
			graphicsAPI = std::make_unique<OpenGLGraphicsAPI>();
			break;
		default:
			LOG_FATAL("Graphics API not supported!");
			abort();
		}
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
