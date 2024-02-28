#include "TestGame.hpp"
#include <iostream>

extern "C" {
	__declspec(dllexport) engine::Game* createGame(engine::Renderer* renderer, engine::Window* window, engine::InputFramework* inputFramework, engine::ResourceManager* resourceManager) {
		engine::Game* game = new engine::TestGame();
		engine::Renderer::instance = renderer;
		engine::Window::instance = window;
		engine::InputFramework::instance = inputFramework;
		engine::Logger::init();
		engine::ResourceManager::instance = resourceManager;
		resourceManager->changeGame(game);
		return game;
	}
}
namespace engine {
	double TestGame::getTargetFrameRate() {
		return 10;
	}

	TestGame::TestGame()
	{
		name = "TestGame";
		glewInit();
	}

	void TestGame::update() {

	}

	void TestGame::initialize() {

		std::shared_ptr<MeshComponent> meshComponent1 = engine::MeshComponent::createMeshFromObjFile("amugus.obj");
		std::shared_ptr<MeshComponent> meshComponent2 = engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE);
		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();

		engine::PhysicsComponent physicsComponentWithGravity = engine::PhysicsComponent();
		physicsComponentWithGravity.setForce(glm::vec3(0, 10, 0));

		engine::PhysicsComponent physicsComponentWithoutGravity = engine::PhysicsComponent(false);
		physicsComponentWithoutGravity.setForce(glm::vec3(0, 0, 0));

		engine::PhysicsComponent physicsComponentWithoutGravityMovingRight = engine::PhysicsComponent(false);
		physicsComponentWithoutGravityMovingRight.mass = 1.f;
		//physicsComponentWithoutGravityMovingRight.setForce(glm::vec3(2, 0, 0));

		engine::PhysicsComponent physicsComponentWithoutGravityMovingLeft = engine::PhysicsComponent(false);
		physicsComponentWithoutGravityMovingLeft.mass = 2.f;
		physicsComponentWithoutGravityMovingLeft.setVelocity(glm::vec3(-5.f, 0, 0));
		//physicsComponentWithoutGravityMovingLeft.setForce(glm::vec3(-2.f, 0, 0));

		engine::PhysicsComponent physicsComponentWithHigherGravity = engine::PhysicsComponent();
		physicsComponentWithHigherGravity.setForce(glm::vec3(0, 10, 0));

		physicsComponentWithHigherGravity.overrideGravityCoefficient = true;
		physicsComponentWithHigherGravity.gravityCoefficient *= 1.2f;

		std::weak_ptr<Material> material = createMaterial("GLOCKEN :)");
		material.lock()->diffuseTexture = loadTexture("glocken.png");
		meshComponent2->setMaterial(material);

		auto boxColliderComponent = engine::BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.f, 1.f, 1.f));
		auto boxColliderOffsetComponent = engine::BoxColliderComponent(glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(1.1f, 1.1f, 1.1f));
		auto sphereColliderComponent = engine::SphereColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.1f, 1.1f, 1.1f));


		GameObject* ball1 = new GameObject();
		ball1->transform.setPosition(glm::vec3(-10, 0, 0));
		ball1->addComponent(meshComponent2);
		ball1->name = "Ball without gravity";
		ball1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));
		ball1->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderComponent));
		ball1->addComponent(std::make_unique<engine::ControllableComponent>());
		addGameObject(std::unique_ptr<GameObject>(ball1));


		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->addComponent(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));


		GameObject* camera = new GameObject();
		camera->transform.setPosition(glm::vec3(0, 0, 10));
		camera->addComponent(std::make_unique<engine::CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));


		GameObject* box1 = new GameObject();
		box1->transform.setPosition(glm::vec3(0, 0, 0));
		box1->addComponent(meshComponent2);
		box1->name = "Box 1";
		box1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravityMovingRight));
		box1->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderComponent));
		addGameObject(std::unique_ptr<GameObject>(box1));


		GameObject* box2 = new GameObject();
		box2->transform.setPosition(glm::vec3(5, 0, 0));
		box2->addComponent(meshComponent2);
		box2->name = "Box 2";
		box2->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravityMovingLeft));
		box2->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderOffsetComponent));
		addGameObject(std::unique_ptr<GameObject>(box2));


		GameObject* box2child1 = new GameObject();
		box2child1->transform.setPosition(glm::vec3(10, 0, 0));
		box2child1->addComponent(meshComponent2);
		box2child1->name = "Box 2 child 1";
		box2child1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravityMovingLeft));
		box2child1->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderOffsetComponent));
		addGameObject(std::unique_ptr<GameObject>(box2child1));
		setParent(std::unique_ptr<GameObject>(box2child1), std::unique_ptr<GameObject>(box2));


		GameObject* box2child2 = new GameObject();
		box2child2->transform.setPosition(glm::vec3(15, 0, 0));
		box2child2->addComponent(meshComponent2);
		box2child2->name = "Box 2 child 2";
		box2child2->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravityMovingLeft));
		box2child2->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderOffsetComponent));
		addGameObject(std::unique_ptr<GameObject>(box2child2));
		setParent(std::unique_ptr<GameObject>(box2child2), std::unique_ptr<GameObject>(box2));


		GameObject* box2child1child = new GameObject();
		box2child1child->transform.setPosition(glm::vec3(12.5f, 0, 0));
		box2child1child->addComponent(meshComponent2);
		box2child1child->name = "Box 2 child 1 child";
		box2child1child->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravityMovingLeft));
		box2child1child->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderOffsetComponent));
		addGameObject(std::unique_ptr<GameObject>(box2child1child));
		setParent(std::unique_ptr<GameObject>(box2child1child), std::unique_ptr<GameObject>(box2child1));


		GameObject* sphere1 = new GameObject();
		sphere1->transform.setPosition(glm::vec3(10, 0, 0));
		sphere1->addComponent(meshComponent2);
		sphere1->name = "Sphere 1";
		sphere1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));
		sphere1->addComponent(std::make_unique<engine::SphereColliderComponent>(sphereColliderComponent));
		addGameObject(std::unique_ptr<GameObject>(sphere1));
	}

}

engine::Game* engine::createGame() {

	return new TestGame();
}