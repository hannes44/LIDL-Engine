#pragma once

#include "GraphicsAPI.hpp"
#include <memory>
#include "Shader.hpp"
#include <vector>
#include "Core/Game.hpp"
#include "RendererSettings.hpp"
#include "Components/MeshComponent.hpp"
#include <optional>

namespace engine
{
	class Renderer
	{
	public:
		// If renderIntoTexture is given, the game will be rendered into the texture instead of the window
		void renderGame(Game* game, CameraComponent* camera, RendererSettings* renderingSettings, glm::vec2 viewPortPos = glm::vec2(0));

		void renderGizmos(Game* game, CameraComponent* camera, RendererSettings* renderingSettings);

		void drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, CameraComponent* camera);

		void drawVector(glm::vec3 dir, glm::vec3 pos, CameraComponent* camera);

		void drawOutlineOfSphere(glm::vec3 position, float radius, CameraComponent* camera);

		// Creates a texture from the renderered GameObject. Can be used to create icons for materials, meshes etc. Position of the GameObject doesn't matter
		// since it will be placed in frame under good lighting conditions
		std::shared_ptr<Texture> renderTextureOfGameObject(GameObject* gameObject);

		// Creates a texture from a rendered mesh using the material. 
		std::shared_ptr<Texture> renderTextureOfMaterial(std::shared_ptr<Material> material);

		void initGraphicsAPI(GraphicsAPIType type);

		std::unique_ptr<GraphicsAPI> graphicsAPI = nullptr;

		GraphicsAPIType getAPIType();

		std::unique_ptr<Shader> baseShader = nullptr;

		static Renderer* getInstance()
		{
			if (instance == nullptr)
			{
				instance = new Renderer();
			}
			return instance;
		}

		// Temporary hack to sync the renderer state with the editor and game
		inline static Renderer* instance;

		// Debug mesh used to render sphere outlines
		std::shared_ptr<MeshComponent> sphereDebugMesh;
		std::shared_ptr<Material> sphereDebugMaterial;

		RendererSettings defaultRendererSettings{};

	private:
		void renderGameObject(CameraComponent* camera, GameObject* gameObject, RendererSettings* rendererSettings);
	};
}