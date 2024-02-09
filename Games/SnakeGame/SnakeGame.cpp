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

		GameObject *head = new GameObject();
		head->transform.setPosition(glm::vec3(0, 0, 0));
		head->name = "Head";
		auto boxColliderComponent = engine::BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.f, 1.f, 1.f));
		auto physicsComponent = engine::PhysicsComponent(false);
		
		
		auto controllableComponent = std::make_shared<engine::ControllableComponent>();
		controllableComponent->movementSpeed = 10.f;
		controllableComponent->movementType = MovementType::Always;
		
		head->addComponent(engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE));
		head->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponent));
		head->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderComponent));
		head->addComponent(controllableComponent);

		addGameObject(std::unique_ptr<GameObject>(head));

		// Set custom physics settings for Snake
		config.physicsSettings.enableGravity = false;
		config.physicsSettings.enableFriction = false;
		config.physicsSettings.enableCollisions = true;
		config.physicsSettings.enableForces = false;
		config.physicsSettings.collisionResolveType = "DISABLED";
	}

}

engine::Game *engine::createGame()
{

	return new SnakeGame();
}