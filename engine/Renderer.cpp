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
	void Renderer::renderGame(GameObject& gameObject, Camera& camera)
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

		glm::mat4 projectionMatrix = camera.getProjectionMatrix();
		glm::mat4 viewMatrix = camera.getViewMatrix();

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
