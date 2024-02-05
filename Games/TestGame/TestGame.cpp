#include "TestGame.hpp"
#include <iostream>

extern "C" {
	__declspec(dllexport) engine::Game* createGame(engine::Renderer* renderer, engine::Window* window, engine::InputFramework* inputFramework) {
		engine::Renderer::instance = renderer;
		engine::Window::instance = window; 
		engine::InputFramework::instance = inputFramework;
		return new engine::TestGame();
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
		Logger::init();


		std::shared_ptr<MeshComponent> meshComponent1 = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");
		std::shared_ptr<MeshComponent> meshComponent2 = engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE);
		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();
		
		engine::PhysicsComponent physicsComponentWithGravity = engine::PhysicsComponent();
		physicsComponentWithGravity.setAcceleration(glm::vec3(0, 10, 0));
		
		engine::PhysicsComponent physicsComponentWithoutGravity = engine::PhysicsComponent(false);
		physicsComponentWithoutGravity.setAcceleration(glm::vec3(0, 0, 0));
		
		engine::PhysicsComponent physicsComponentWithHigherGravity = engine::PhysicsComponent();
		physicsComponentWithHigherGravity.setAcceleration(glm::vec3(0, 10, 0));
		
		physicsComponentWithHigherGravity.overrideGravityCoefficient = true;
		physicsComponentWithHigherGravity.gravityCoefficient *= 1.2f;

		meshComponent2->material.diffuseTexture = loadTexture("glocken.png");

		auto boxColliderComponent = engine::BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.f, 1.f, 1.f));
		auto boxColliderOffsetComponent = engine::BoxColliderComponent(glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(1.1f, 1.1f, 1.1f));
		auto sphereColliderComponent = engine::SphereColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.1f, 1.1f, 1.1f));
		
		Debug::Log("Hello World");

		/*
		GameObject* sphere1 = new GameObject();
		sphere1->transform.setScale(glm::vec3(2, 2, 2));
		sphere1->transform.setPosition(glm::vec3(10, 10, 0));
		sphere1->addComponent(meshComponent1);
		sphere1->name = "SIGMA AMUGUS";
		addGameObject(std::unique_ptr<GameObject>(sphere1));

		GameObject* sphere2 = new GameObject();
		float width = 5;
		float height = width / 0.52f;
		float depth = width / 2.f;
		sphere2->transform.setScale(glm::vec3(width, height, depth));
		sphere2->transform.setPosition(glm::vec3(15, 10, 0));
		sphere2->addComponent(meshComponent2);
		sphere2->name = "BOMPASPY";
		addGameObject(std::unique_ptr<GameObject>(sphere2));


		*/

		GameObject* ball1 = new GameObject();
		ball1->transform.setPosition(glm::vec3(-10, 0, 0));
		ball1->addComponent(meshComponent2);
		ball1->name = "Ball with gravity";
		ball1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithGravity));
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
		box1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));
		box1->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderComponent));

		addGameObject(std::unique_ptr<GameObject>(box1));

		
		GameObject* box2 = new GameObject();
		box2->transform.setPosition(glm::vec3(5, 0, 0));
		box2->addComponent(meshComponent2);
		box2->name = "Box 2";
		box2->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));
		box2->addComponent(std::make_unique<engine::BoxColliderComponent>(boxColliderOffsetComponent));

		addGameObject(std::unique_ptr<GameObject>(box2));

		
		GameObject* sphere1 = new GameObject();
		sphere1->transform.setPosition(glm::vec3(10, 0, 0));
		sphere1->addComponent(meshComponent2);
		sphere1->name = "Sphere 1";
		sphere1->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));
		sphere1->addComponent(std::make_unique<engine::SphereColliderComponent>(sphereColliderComponent));

		addGameObject(std::unique_ptr<GameObject>(sphere1));

		/*
		GameObject* ball2 = new GameObject();
		ball2->transform.setPosition(glm::vec3(-10, 0, 0));
		ball2->addComponent(meshComponent2);
		ball2->name = "Ball without gravity";
		ball2->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));

		addGameObject(std::unique_ptr<GameObject>(ball2));


		GameObject* ball3 = new GameObject();
		ball3->transform.setPosition(glm::vec3(-20, 0, 0));
		ball3->addComponent(meshComponent2);
		ball3->name = "Ball with higher gravity";
		ball3->addComponent(std::make_unique<engine::PhysicsComponent>(physicsComponentWithHigherGravity));

		addGameObject(std::unique_ptr<GameObject>(ball3));
		*/
	}

}

engine::Game* engine::createGame() {

	return new TestGame();
}