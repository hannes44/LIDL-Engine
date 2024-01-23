#include "Renderer.hpp"
#include "Logger.hpp"
#include "OpenGLGraphicsAPI.hpp"
#include "GameObject.hpp"
#include "MeshComponent.hpp"
#include "Window.hpp"
#include "PointLightComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


namespace engine
{
	void Renderer::renderGame(std::vector<GameObject*> gameObjects, Camera& camera)
	{
		graphicsAPI->setClearColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		graphicsAPI->setCullFace(false);

		graphicsAPI->setDepthTest(true);

		baseShader->bind();

		glm::mat4 projectionMatrix = camera.getProjectionMatrix();
		glm::mat4 viewMatrix = camera.getViewMatrix();


		int lightIndex = 0;
		// TODO: There should be a list of all the lights in the game to avoid this loop
		for (auto gameObject : gameObjects)
		{
			for (auto component : gameObject->components)
			{
				if (dynamic_cast<PointLightComponent*>(component.get()))
				{
					PointLightComponent* light = dynamic_cast<PointLightComponent*>(component.get());

					std::string index = "[" + std::to_string(lightIndex) + "]";
					baseShader->setVec3(("pointLights" + index + ".position").c_str(), gameObject->transform.getPosition().x, gameObject->transform.getPosition().y, gameObject->transform.getPosition().z);
					baseShader->setVec3(("pointLights" + index + ".ambient").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setVec3(("pointLights" + index + ".diffuse").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setVec3(("pointLights" + index + ".specular").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setFloat(("pointLights" + index + ".constant").c_str(), 10);
					baseShader->setFloat(("pointLights" + index + ".linear").c_str(), 0);
					baseShader->setFloat(("pointLights" + index + ".quadratic").c_str(), 0);

					lightIndex++;
				}
			}
		}

		baseShader->setInt("numLights", lightIndex);
		baseShader->setVec3("viewPos", camera.translation.x, camera.translation.y, camera.translation.z);


		for (auto& gameObject : gameObjects)
		{
			MeshComponent* meshComponent = nullptr;

			// TODO: Only gameobjects with a mesh should be sent to the renderer to aviod looping
			for (auto component : gameObject->components)
			{
				if (dynamic_cast<MeshComponent*>(component.get()))
				{
					meshComponent = dynamic_cast<MeshComponent*>(component.get());
				}
			}

			if (meshComponent == nullptr)
			{
				LOG_INFO("No mesh component found!");
				return;
			}

			glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * gameObject->transform.transformMatrix;
			Renderer::baseShader->setMat4("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);
			Renderer::baseShader->setMat4("modelMatrix", &gameObject->transform.transformMatrix[0].x);

			graphicsAPI->drawIndexed(meshComponent->vertexArray.get(), meshComponent->indices.size());
		}

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
	std::unique_ptr<Shader> Renderer::baseShader;
}
