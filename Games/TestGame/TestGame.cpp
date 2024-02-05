#include "TestGame.hpp"
#include <iostream>

extern "C" {
	__declspec(dllexport) engine::Game* createGame() {
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
	}

	void TestGame::update() {

	}

	void TestGame::initialize() {
		Logger::init();


		std::shared_ptr<MeshComponent> meshComponent1 = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");
		std::shared_ptr<MeshComponent> meshComponent2 = engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE);
		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();
		engine::PhysicsComponent physicsComponentWithGravity = engine::PhysicsComponent();
		physicsComponentWithGravity.setVelocity(glm::vec3(0, 10, 0));
		
		engine::PhysicsComponent physicsComponentWithoutGravity = engine::PhysicsComponent(true);
		physicsComponentWithoutGravity.setVelocity(glm::vec3(0, 0, 0));
		
		engine::PhysicsComponent physicsComponentWithHigherGravity = engine::PhysicsComponent();
		physicsComponentWithHigherGravity.setVelocity(glm::vec3(0, 10, 0));
		
		physicsComponentWithHigherGravity.overrideGravityCoefficient = true;
		physicsComponentWithHigherGravity.gravityCoefficient *= 1.2f;

		//meshComponent2->material.diffuseTexture = loadTexture("glocken.png");

		Debug::Log("Hello World");

		/*
		GameObject* sphere1 = new GameObject();
		sphere1->transform.setScale(glm::vec3(2, 2, 2));
		sphere1->transform.setPosition(glm::vec3(10, 10, 0));
		sphere1->components.push_back(meshComponent1);
		sphere1->name = "SIGMA AMUGUS";
		addGameObject(std::unique_ptr<GameObject>(sphere1));

		GameObject* sphere2 = new GameObject();
		float width = 5;
		float height = width / 0.52f;
		float depth = width / 2.f;
		sphere2->transform.setScale(glm::vec3(width, height, depth));
		sphere2->transform.setPosition(glm::vec3(15, 10, 0));
		sphere2->components.push_back(meshComponent2);
		sphere2->name = "BOMPASPY";
		addGameObject(std::unique_ptr<GameObject>(sphere2));


		*/
		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->components.push_back(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));

		GameObject* camera = new GameObject();
		camera->transform.setPosition(glm::vec3(0, 0, 10));
		camera->components.push_back(std::make_unique<engine::CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));


		GameObject* ball1 = new GameObject();
		ball1->transform.setPosition(glm::vec3(0, 0, 0));
		ball1->components.push_back(meshComponent2);
		ball1->name = "Ball with gravity";

		ball1->components.push_back(std::make_unique<engine::PhysicsComponent>(physicsComponentWithGravity));

		addGameObject(std::unique_ptr<GameObject>(ball1));


		GameObject* ball2 = new GameObject();
		ball2->transform.setPosition(glm::vec3(-10, 0, 0));
		ball2->components.push_back(meshComponent2);
		ball2->name = "Ball without gravity";
		

		ball2->components.push_back(std::make_unique<engine::PhysicsComponent>(physicsComponentWithoutGravity));

		addGameObject(std::unique_ptr<GameObject>(ball2));


		GameObject* ball3 = new GameObject();
		ball3->transform.setPosition(glm::vec3(-20, 0, 0));
		ball3->components.push_back(meshComponent2);
		ball3->name = "Ball with higher gravity";

		ball3->components.push_back(std::make_unique<engine::PhysicsComponent>(physicsComponentWithHigherGravity));

		addGameObject(std::unique_ptr<GameObject>(ball3));
	}

}

engine::Game* engine::createGame() {

	return new TestGame();
}