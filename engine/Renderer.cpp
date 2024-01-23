#include "Renderer.hpp"
#include "Logger.hpp"
#include "OpenGLGraphicsAPI.hpp"
#include "GameObject.hpp"
#include "MeshComponent.hpp"
#include "Window.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


namespace engine
{
	void Renderer::renderGame(GameObject& gameObject)
	{
		graphicsAPI->setClearColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		baseShader->bind();

		// TEMPORARY
		glDisable(GL_CULL_FACE);

		MeshComponent* meshComponent = nullptr;

		// TODO: Only gameobjects with a mesh should be sent to the renderer to aviod looping
		for (auto component : gameObject.components)
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

		float fov = 90;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		int width;
		int height;
		Window::getInstance().getWindowSize(&width, &height);


		float aspectRatio = float(width) / float(height);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		glm::vec3 direction = glm::normalize(glm::vec3(-1,-1,-1));

		glm::vec3 translation = glm::vec3(15, 15 ,15);

		glm::vec3 worldUp = glm::vec3(0, 1, 0);
		glm::vec3 cameraRight = glm::normalize(glm::cross(direction, worldUp));
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, direction));

		glm::mat3 cameraBaseVectorsWorldSpace(cameraRight, cameraUp, -direction);
		glm::mat4 cameraRotation = glm::mat4(transpose(cameraBaseVectorsWorldSpace));
		glm::mat4 viewMatrix = cameraRotation * glm::translate(-translation);

		//for (auto& model : scene->models)
	//	{
			// Bind shader TODO

		glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * gameObject.transform.transformMatrix;
		Renderer::baseShader->setMat4("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);
		Renderer::baseShader->setMat4("modelMatrix", &gameObject.transform.transformMatrix[0].x);
		//	Renderer::baseShader->setInt("hasMaterial", !model->material.expired());

			/*
	
			if (auto lockedMaterial = model->material.lock())
			{
				Renderer::baseShader->setFloat("material.shininess", lockedMaterial->shininess);
				if (!lockedMaterial->diffuseTexture.expired())
				{
					Renderer::baseShader->setInt("material.diffuseTexture", 0);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, lockedMaterial->diffuseTexture.lock()->textureIdOpenGL);
				}
				if (!lockedMaterial->specularTexture.expired())
				{
					Renderer::baseShader->setInt("material.specularTexture", 1);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, lockedMaterial->specularTexture.lock()->textureIdOpenGL);
				}

				Renderer::baseShader->setVec3("material.baseColor", lockedMaterial->baseColor.x, lockedMaterial->baseColor.y, lockedMaterial->baseColor.z);
				Renderer::baseShader->setInt("material.hasDiffuseTexture", !lockedMaterial->diffuseTexture.expired());
				Renderer::baseShader->setInt("material.hasSpecularTexture", !lockedMaterial->specularTexture.expired());
			}	
			*/

			graphicsAPI->drawIndexed(meshComponent->vertexArray.get(), meshComponent->indices.size());

			//model->shader->unbind();
		//}




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
	std::unique_ptr<Shader> Renderer::baseShader;
}
