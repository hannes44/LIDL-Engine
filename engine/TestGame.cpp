#include <iostream>
#include "InputFramework.hpp"
#include "Window.hpp"
#include "Game.hpp"
#include "Cube.hpp"
#include "GameObject.hpp"
#include "TestGame.hpp"
#include "Bootstrap.hpp"
#include "MeshComponent.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "PointLightComponent.hpp"


namespace engine {
	double TestGame::getTargetFrameRate() {
		return 0;
	}

	void TestGame::update() {

		InputFramework::getInstance().getInput();
	
		// ----------------------------------------------------------
		// CONSOLE OUTPUT
		// ----------------------------------------------------------
		/*

		Bootstrap::getInstance().getDisplay().clear();

		for (auto gameObject : gameObjects) {
			Bootstrap::getInstance().getDisplay().drawGameObject(gameObject);
		}

		if (frameCount % 2 == 0) {
			Bootstrap::getInstance().getDisplay().drawText("Frame: " + std::to_string(frameCount), 0, 0, Color::WHITE);
		}
		else {
			std::string size = std::to_string(Bootstrap::getInstance().getDisplay().getWidth()) + "x" + std::to_string(Bootstrap::getInstance().getDisplay().getHeight());
			Bootstrap::getInstance().getDisplay().drawText("Terminal size: " + size, 0, 1, Color::RED);
		}

		Bootstrap::getInstance().getDisplay().show();
				*/
		// ----------------------------------------------------------
		// SDL WINDOW OUTPUT
		// ----------------------------------------------------------

		gui.renderNewFrame();

		Renderer::renderGame(this, camera);
		
		gui.endFrame();

		Window::getInstance().newFrame();
	}

	void TestGame::initialize() {
		// Initialize the InputFramework and InputSystem
		InputFramework::getInstance().initialize();


		engine::MeshComponent meshComponent1 = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");
		engine::MeshComponent meshComponent2 = engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE);
		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();

		meshComponent2.material.diffuseTexture = loadTexture("bompaspy.png");

		GameObject* sphere1 = new GameObject();
		sphere1->transform.setScale(glm::vec3(2, 2, 2));
		sphere1->transform.setPosition(glm::vec3(10, 10, 0));
		sphere1->components.push_back(std::make_unique<engine::MeshComponent>(meshComponent1));
		sphere1->name = "SIGMA AMUGUS";
		addGameObject(std::unique_ptr<GameObject>(sphere1));

		GameObject* sphere2 = new GameObject();
		sphere2->transform.setScale(glm::vec3(15, 15, 15));
		sphere2->transform.setPosition(glm::vec3(15, 10, 0));
		sphere2->components.push_back(std::make_unique<engine::MeshComponent>(meshComponent2));
		sphere2->name = "BOMPASPY";
		addGameObject(std::unique_ptr<GameObject>(sphere2));

		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->components.push_back(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));
	}
}