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
#include "Components/SpotLightComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "Components/PhysicsComponent.hpp"


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

		int pointLightIndex = 0;
		int spotLightIndex = 0;

		// TODO: There should be a list of all the lights in the game to avoid this loop
		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			for (auto component : gameObject->getComponents())
			{
				if (dynamic_cast<PointLightComponent*>(component.get()))
				{
					PointLightComponent* light = dynamic_cast<PointLightComponent*>(component.get());

					std::string index = "[" + std::to_string(pointLightIndex) + "]";
					glm::vec3 gameObjectPosition = gameObject->getGlobalTransform().getPosition();
					baseShader->setVec3(("pointLights" + index + ".position").c_str(), gameObjectPosition.x, gameObjectPosition.y, gameObjectPosition.z);
					baseShader->setVec3(("pointLights" + index + ".ambient").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setVec3(("pointLights" + index + ".diffuse").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setVec3(("pointLights" + index + ".specular").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setFloat(("pointLights" + index + ".constant").c_str(), light->constant);
					baseShader->setFloat(("pointLights" + index + ".linear").c_str(), light->linear);
					baseShader->setFloat(("pointLights" + index + ".quadratic").c_str(), light->quadratic);

					pointLightIndex++;
				}
				if (dynamic_cast<SpotLightComponent*>(component.get()))
				{
					SpotLightComponent* light = dynamic_cast<SpotLightComponent*>(component.get());

					std::string index = "[" + std::to_string(spotLightIndex) + "]";
					glm::vec3 gameObjectPosition = gameObject->getGlobalTransform().getPosition();
					glm::vec3 gameObjectDirection = gameObject->getGlobalTransform().getLocalForward();
					baseShader->setVec3(("spotLights" + index + ".position").c_str(), gameObjectPosition.x, gameObjectPosition.y, gameObjectPosition.z);
					baseShader->setVec3(("spotLights" + index + ".direction").c_str(), gameObjectDirection.x, gameObjectDirection.y, gameObjectDirection.z);
					baseShader->setVec3(("spotLights" + index + ".ambient").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setVec3(("spotLights" + index + ".diffuse").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setVec3(("spotLights" + index + ".specular").c_str(), light->color.x, light->color.y, light->color.z);
					baseShader->setFloat(("spotLights" + index + ".constant").c_str(), light->constant);
					baseShader->setFloat(("spotLights" + index + ".linear").c_str(), light->linear);
					baseShader->setFloat(("spotLights" + index + ".quadratic").c_str(), light->quadratic);
					baseShader->setFloat(("spotLights" + index + ".cutOff").c_str(), glm::cos(glm::radians(light->cutOffAngle)));
					baseShader->setFloat(("spotLights" + index + ".outerCutOff").c_str(), glm::cos(glm::radians(light->outerCutOffAngle)));

					spotLightIndex++;
				}
			}
		}

		baseShader->setInt("numPointLights", pointLightIndex);
		baseShader->setInt("numSpotLights", spotLightIndex);
		baseShader->setVec3("viewPos", camera->getTransform().getPosition().x, camera->getTransform().getPosition().y, camera->getTransform().getPosition().z);
		baseShader->setVec3("backgroundColor", renderingSettings->backgroundColor.x, renderingSettings->backgroundColor.y, renderingSettings->backgroundColor.z);
		baseShader->setInt("enableFog", renderingSettings->enableFog);
		baseShader->setFloat("startFogDistance", renderingSettings->startFogDistance);
		baseShader->setFloat("endFogDistance", renderingSettings->endFogDistance);

		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			std::shared_ptr<MeshComponent> meshComponent = gameObject->getComponent<MeshComponent>();

			if (meshComponent == nullptr)
				continue;

			// Skip rendering meshes that should be rendered in front of the camera
			// They need to be rendered last to avoid clipping into other objects
			if (meshComponent->renderFromCameraTransform)
				continue;

			renderGameObject(game, camera, gameObject.get());
		}
		
		// Can be optimized to avoid looping through all game objects twice
		for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			std::shared_ptr<MeshComponent> meshComponent = gameObject->getComponent<MeshComponent>();

			if (meshComponent == nullptr)
				continue;

			if (!meshComponent->renderFromCameraTransform)
				continue;

			renderGameObject(game, camera, gameObject.get());
		}

	}

	void Renderer::drawVector(glm::vec3 dir, glm::vec3 pos, CameraComponent* camera)
	{
		// 0 - 1
		const float angle = 0.5f;

		// 0 - 1
		const float headLength = 0.7f;

		// > 0
		const float length = 3.f;

		dir = glm::normalize(dir);

		glm::vec3 color = glm::vec3(0, 1, 0);
		
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 side = glm::vec3(0, 0, 1);
		glm::vec3 end = pos + dir * length;

		glm::vec3 right = (1.f + angle) * length * glm::normalize((dir == up || dir == -up) ? glm::cross(dir, side) : glm::cross(dir, up));
		glm::vec3 midRight = pos + (pos - right) * 0.5f;
		glm::vec3 midLeft = pos + (pos + right) * 0.5f;

		drawLine(pos, end, color, camera);
		drawLine(end, end + glm::normalize(midRight - end) * headLength, color, camera);
		drawLine(end, end + glm::normalize(midLeft - end) * headLength, color, camera);
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

		auto cameraGO = std::make_shared<GameObject>();
		auto camera = std::make_shared<CameraComponent>();
		cameraGO->addComponent(camera);

		cameraGO->transform.setPosition(glm::vec3(4, 0, 0));
		cameraGO->transform.setRotationFromDirection(glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0));

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

		glm::mat4 projectionMatrix = camera->getProjectionMatrix(width, height);

		PointLightComponent light = PointLightComponent();
		light.color = glm::vec3(1, 1, 1);
		glm::vec3 lightPosition = glm::vec3(2, 2, 0);

		std::string index = "[0]";
		baseShader->setVec3(("pointLights" + index + ".position").c_str(), lightPosition.x, lightPosition.y, lightPosition.z);
		baseShader->setVec3(("pointLights" + index + ".ambient").c_str(), light.color.x, light.color.y, light.color.z);
		baseShader->setVec3(("pointLights" + index + ".diffuse").c_str(), light.color.x, light.color.y, light.color.z);
		baseShader->setVec3(("pointLights" + index + ".specular").c_str(), light.color.x, light.color.y, light.color.z);
		baseShader->setFloat(("pointLights" + index + ".constant").c_str(), light.constant);
		baseShader->setFloat(("pointLights" + index + ".linear").c_str(), light.linear);
		baseShader->setFloat(("pointLights" + index + ".quadratic").c_str(), light.quadratic);

		baseShader->setInt("numPointLights", 1);

		baseShader->setVec3("viewPos", cameraGO->transform.getPosition().x, cameraGO->transform.getPosition().y, cameraGO->transform.getPosition().z);

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

		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::mat4 gameObjectTransformMatrix = gameObject->getGlobalTransform().transformMatrix;
		glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * gameObjectTransformMatrix;
		glm::mat4 modelViewMatrix = viewMatrix * gameObjectTransformMatrix;
		Renderer::baseShader->setMat4("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(gameObjectTransformMatrix));
		Renderer::baseShader->setMat4("normalMatrix", &normalMatrix[0].x);
		modelViewMatrix = viewMatrix * gameObjectTransformMatrix;
		Renderer::baseShader->setMat4("modelMatrix", &gameObjectTransformMatrix[0].x);

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
	void Renderer::renderGameObject(Game* game, CameraComponent* camera, GameObject* gameObject)
	{
		std::shared_ptr<MeshComponent> meshComponent = gameObject->getComponent<MeshComponent>();

		if (meshComponent == nullptr)
			return;

		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::mat4 gameObjectTransformMatrix = gameObject->getGlobalTransform().transformMatrix;
		glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * gameObjectTransformMatrix;
		glm::mat4 modelViewMatrix = viewMatrix * gameObjectTransformMatrix;

		if (game->running && meshComponent->renderFromCameraTransform)
		{
			modelViewProjectionMatrix = projectionMatrix * viewMatrix * camera->getTransform().transformMatrix * gameObjectTransformMatrix;
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		Renderer::baseShader->setMat4("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(gameObjectTransformMatrix));
		Renderer::baseShader->setMat4("normalMatrix", &normalMatrix[0].x);
		modelViewMatrix = viewMatrix * gameObjectTransformMatrix;
		Renderer::baseShader->setMat4("modelMatrix", &gameObjectTransformMatrix[0].x);

		Material* material = meshComponent->getMaterial();
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
