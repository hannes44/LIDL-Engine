#include "Renderer.hpp"
#include "Core/Logger.hpp"
#include "Renderer/OpenGL/OpenGLGraphicsAPI.hpp"
#include "Core/GameObject.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/Window.hpp"
#include "Components/PointLightComponent.hpp"
#include "Core/Game.hpp"
#include "Core/Camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


namespace engine
{
	void Renderer::renderGame(Game* game, Camera* camera, RendererSettings* renderingSettings)
	{
		int width, height;
		Window::getInstance().getWindowSize(&width, &height);

		graphicsAPI->setViewport(0, 0, width, height);

		graphicsAPI->setClearColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		graphicsAPI->setCullFace(renderingSettings->enableFaceCulling);

		graphicsAPI->setDepthTest(renderingSettings->enableDepthTest);

		graphicsAPI->setDrawTriangleOutline(renderingSettings->drawWireframe);

		if (renderingSettings->useMultiSampling)
			glEnable(GL_MULTISAMPLE);
		else 
			glDisable(GL_MULTISAMPLE);

		baseShader->bind();

		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		glm::mat4 viewMatrix = camera->getViewMatrix();


		int lightIndex = 0;
		// TODO: There should be a list of all the lights in the game to avoid this loop
		for (const auto& [gameObjectId, gameObject] : game->gameObjects) 
		{
			for (auto component : gameObject->getComponents())
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
		baseShader->setVec3("viewPos", camera->translation.x, camera->translation.y, camera->translation.z);


		for (const auto& [gameObjectId, gameObject] : game->gameObjects)
		{
			MeshComponent* meshComponent = nullptr;

			// TODO: Only gameobjects with a mesh should be sent to the renderer to aviod looping
			for (auto component : gameObject->getComponents())
			{
				if (dynamic_cast<MeshComponent*>(component.get()))
				{
					meshComponent = dynamic_cast<MeshComponent*>(component.get());
					break;
				}
			}

			if (meshComponent == nullptr)
				continue;
			

			glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * gameObject->transform.transformMatrix;
			Renderer::baseShader->setMat4("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);
			Renderer::baseShader->setMat4("modelMatrix", &gameObject->transform.transformMatrix[0].x);

			// Material
			Renderer::baseShader->setFloat("material.shininess", meshComponent->material.shininess);
			Renderer::baseShader->setVec3("material.baseColor", meshComponent->material.baseColor.x, meshComponent->material.baseColor.y, meshComponent->material.baseColor.z);
			Renderer::baseShader->setInt("material.hasDiffuseTexture", !meshComponent->material.diffuseTexture.expired());
			Renderer::baseShader->setInt("material.hasSpecularTexture", !meshComponent->material.specularTexture.expired());

			if (!meshComponent->material.diffuseTexture.expired())
			{
				Renderer::baseShader->setInt("material.diffuseTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, meshComponent->material.diffuseTexture.lock()->textureIDOpenGL);
			}
			if (!meshComponent->material.specularTexture.expired())
			{
				Renderer::baseShader->setInt("material.specularTexture", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, meshComponent->material.specularTexture.lock()->textureIDOpenGL);
			}

			graphicsAPI->drawIndexed(meshComponent->vertexArray.get(), meshComponent->indices.size());
		}

	}

	void Renderer::renderGizmos(Game* game, Camera* camera, RendererSettings* renderingSettings)
	{
		for (const auto& [gameObjectId, gameObject] : game->gameObjects)
		{
			ColliderComponent* colliderComponent = nullptr;

			for (auto component : gameObject->getComponents())
			{
				if (dynamic_cast<ColliderComponent*>(component.get()))
				{
					colliderComponent = dynamic_cast<ColliderComponent*>(component.get());
					break;
				}
			}

			if (colliderComponent == nullptr)
				continue;
			
			colliderComponent->drawCollider(camera);
		}

	}

	void Renderer::drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, Camera* camera)
	{
		glm::mat4 modelViewProjection = camera->getProjectionMatrix() * camera->getViewMatrix() * glm::mat4(1.0f);
		graphicsAPI->drawLine(start, end, color, modelViewProjection);
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
