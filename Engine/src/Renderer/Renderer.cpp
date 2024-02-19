#include "Renderer.hpp"
#include "Core/Logger.hpp"
#include "Renderer/OpenGL/OpenGLGraphicsAPI.hpp"
#include "Core/GameObject.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/Window.hpp"
#include "Components/PointLightComponent.hpp"
#include "Core/Game.hpp"
#include "Components/CameraComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


namespace engine
{
	void Renderer::renderGame(Game* game, CameraComponent* camera, RendererSettings* renderingSettings)
	{
		if (!camera)
		{
			LOG_WARN("RenderGame: No camera found! Add camera component to game");
			graphicsAPI->setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			// TODO: Render text to the screen telling user to add a camera
			return;
		}
		int width, height;
		Window::getInstance().getWindowSize(&width, &height);

		graphicsAPI->setViewport(0, 0, width, height);

		graphicsAPI->setClearColor(glm::vec4(renderingSettings->backgroundColor.x, renderingSettings->backgroundColor.y, renderingSettings->backgroundColor.z, 1.0f));

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
		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
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


		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			MeshComponent* meshComponent = nullptr;

			// TODO: Only GameObjects with a mesh should be sent to the renderer to avoid looping
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

			Material* material = meshComponent->getMaterial();
			// Material
			Renderer::baseShader->setFloat("material.shininess", material->shininess);
			Renderer::baseShader->setVec3("material.baseColor", material->baseColor.x, material->baseColor.y, material->baseColor.z);
			Renderer::baseShader->setInt("material.hasDiffuseTexture", !material->diffuseTexture.expired());
			Renderer::baseShader->setInt("material.hasSpecularTexture", !material->specularTexture.expired());

			if (!material->diffuseTexture.expired())
			{
				Renderer::baseShader->setInt("material.diffuseTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->diffuseTexture.lock()->textureIDOpenGL);
			}
			if (!material->specularTexture.expired())
			{
				Renderer::baseShader->setInt("material.specularTexture", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->specularTexture.lock()->textureIDOpenGL);
			}

			graphicsAPI->drawIndexed(meshComponent->getVertexArray().get(), meshComponent->indices.size());
		}

	}

	void Renderer::renderGizmos(Game* game, CameraComponent* camera, RendererSettings* renderingSettings)
	{
		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
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

			if (renderingSettings->drawBoundingBoxes)
				colliderComponent->drawCollider(camera);
		}

	}

	void Renderer::drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, CameraComponent* camera)
	{
		glm::mat4 modelViewProjection = camera->getProjectionMatrix() * camera->getViewMatrix() * glm::mat4(1.0f);
		graphicsAPI->drawLine(start, end, color, modelViewProjection);
	}

	// TODO: Don't use OpenGL code here
	std::shared_ptr<Texture> Renderer::renderTextureOfGameObject(GameObject* gameObject)
	{
		// Resolution of the texture
		int width = 1000;
		int height = 1000;

		RendererSettings renderingSettings{};

		GLuint textureFrameBuffer = 0;
		glGenFramebuffers(1, &textureFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, textureFrameBuffer);

		GLuint renderedTexture;
		glGenTextures(1, &renderedTexture);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// The depth buffer
		GLuint depthrenderbuffer;
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachment #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_FATAL("Framebuffer is not complete!");
			return nullptr;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, textureFrameBuffer);


		CameraComponent camera = CameraComponent();

		camera.translation = glm::vec3(2.5, 0, 2.5);
		camera.direction = glm::vec3(-1, 0, -1);

		graphicsAPI->setViewport(0, 0, width, height);

		graphicsAPI->setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		graphicsAPI->setCullFace(renderingSettings.enableFaceCulling);

		graphicsAPI->setDepthTest(renderingSettings.enableDepthTest);

		graphicsAPI->setDrawTriangleOutline(renderingSettings.drawWireframe);

		if (renderingSettings.useMultiSampling)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);

		baseShader->bind();

		glm::mat4 projectionMatrix = camera.getProjectionMatrix(width, height);
		glm::mat4 viewMatrix = camera.getViewMatrix();

		PointLightComponent light = PointLightComponent();
		light.color = glm::vec3(1, 1, 1);
		glm::vec3 lightPosition = glm::vec3(2, 2, 0);

		std::string index = "[0]";
		baseShader->setVec3(("pointLights" + index + ".position").c_str(), lightPosition.x, lightPosition.y, lightPosition.z);
		baseShader->setVec3(("pointLights" + index + ".ambient").c_str(), light.color.x, light.color.y, light.color.z);
		baseShader->setVec3(("pointLights" + index + ".diffuse").c_str(), light.color.x, light.color.y, light.color.z);
		baseShader->setVec3(("pointLights" + index + ".specular").c_str(), light.color.x, light.color.y, light.color.z);
		baseShader->setFloat(("pointLights" + index + ".constant").c_str(), 10);
		baseShader->setFloat(("pointLights" + index + ".linear").c_str(), 0);
		baseShader->setFloat(("pointLights" + index + ".quadratic").c_str(), 0);


		baseShader->setInt("numLights", 1);

		baseShader->setVec3("viewPos", camera.translation.x, camera.translation.y, camera.translation.z);



		MeshComponent* meshComponent = nullptr;

		// TODO: Only GameObjects with a mesh should be sent to the renderer to avoid looping
		for (auto component : gameObject->getComponents())
		{
			if (dynamic_cast<MeshComponent*>(component.get()))
			{
				meshComponent = dynamic_cast<MeshComponent*>(component.get());
				break;
			}
		}

		if (meshComponent == nullptr)
		{
			LOG_ERROR("No mesh component found in game object, can't create texture of game object");
			return nullptr;
		}

		glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * gameObject->transform.transformMatrix;
		Renderer::baseShader->setMat4("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);
		Renderer::baseShader->setMat4("modelMatrix", &gameObject->transform.transformMatrix[0].x);

		Material* material = meshComponent->getMaterial();
		// Material
		Renderer::baseShader->setFloat("material.shininess", material->shininess);
		Renderer::baseShader->setVec3("material.baseColor", material->baseColor.x, material->baseColor.y, material->baseColor.z);
		Renderer::baseShader->setInt("material.hasDiffuseTexture", !material->diffuseTexture.expired());
		Renderer::baseShader->setInt("material.hasSpecularTexture", !material->specularTexture.expired());

		if (!material->diffuseTexture.expired())
		{
			Renderer::baseShader->setInt("material.diffuseTexture", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuseTexture.lock()->textureIDOpenGL);
		}
		if (!material->specularTexture.expired())
		{
			Renderer::baseShader->setInt("material.specularTexture", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->specularTexture.lock()->textureIDOpenGL);
		}

		graphicsAPI->drawIndexed(meshComponent->getVertexArray().get(), meshComponent->indices.size());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::create());
		texture->textureIDOpenGL = renderedTexture;
		return texture;
	}

	std::shared_ptr<Texture> Renderer::renderTextureOfMaterial(std::shared_ptr<Material> material)
	{
		GameObject materialSphereGameObject = GameObject();
		materialSphereGameObject.addComponent(MeshComponent::createMeshFromObjFile("amugus.obj", false));
		materialSphereGameObject.getComponent<MeshComponent>()->setMaterial(std::shared_ptr<Material>(material));

		return renderTextureOfGameObject(&materialSphereGameObject);
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
}
