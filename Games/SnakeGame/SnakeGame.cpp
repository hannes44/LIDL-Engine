#include "SnakeGame.hpp"
#include <iostream>

extern "C"
{
	__declspec(dllexport) engine::Game *createGame(engine::Renderer *renderer, engine::Window *window, engine::InputFramework *inputFramework, engine::ResourceManager *resourceManager)
	{
		engine::Game *game = new engine::SnakeGame();
		engine::Renderer::instance = renderer;
		engine::Window::instance = window;
		engine::InputFramework::instance = inputFramework;
		engine::Logger::init();
		engine::ResourceManager::instance = resourceManager;
		resourceManager->changeGame(game);
		return game;
	}
}
namespace engine
{
	double SnakeGame::getTargetFrameRate()
	{
		return 10;
	}

	SnakeGame::SnakeGame()
	{
		name = "SnakeGame";
		glewInit();
	}

	void SnakeGame::update()
	{
	}

	void SnakeGame::initialize()
	{
		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();

		GameObject *light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->addComponent(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));

		GameObject *camera = new GameObject();
		camera->transform.setPosition(glm::vec3(0, 0, 10));
		camera->addComponent(std::make_unique<engine::CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));

		GameObject *sphere1 = new GameObject();
		sphere1->transform.setPosition(glm::vec3(10, 0, 0));
		sphere1->name = "Gunilla gorilla";
		sphere1->addComponent(engine::MeshComponent::createPrimative(PrimativeMeshType::SPHERE));

		addGameObject(std::unique_ptr<GameObject>(sphere1));
	}

}

engine::Game *engine::createGame()
{

	return new SnakeGame();
}