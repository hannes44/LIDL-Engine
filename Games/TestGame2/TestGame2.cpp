#include "TestGame2.hpp"

extern "C" {
	__declspec(dllexport) engine::Game* createGame() {
		return new engine::TestGame2();
	}
}
namespace engine {
	double TestGame2::getTargetFrameRate() {
		return 10;
	}

	void TestGame2::update() {

	}

	void TestGame2::initialize() {
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

		
		GameObject* sphere1 = new GameObject();
		sphere1->transform.setScale(glm::vec3(2, 2, 2));
		sphere1->transform.setPosition(glm::vec3(10, 10, 0));
		sphere1->components.push_back(meshComponent1);
		sphere1->name = "SIGMA AMUGUS";
		addGameObject(std::unique_ptr<GameObject>(sphere1));

		GameObject* camera = new GameObject();
		camera->transform.setPosition(glm::vec3(0, 0, 10));
		camera->components.push_back(std::make_unique<engine::CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));

		GameObject* sphere2 = new GameObject();
		float width = 5;
		float height = width / 0.52f;
		float depth = width / 2.f;
		sphere2->transform.setScale(glm::vec3(width, height, depth));
		sphere2->transform.setPosition(glm::vec3(15, 10, 0));
		sphere2->components.push_back(meshComponent2);
		sphere2->name = "BOMPASPY";
		addGameObject(std::unique_ptr<GameObject>(sphere2));


		
		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->components.push_back(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));


	}

}

engine::Game* engine::createGame() {

	return new TestGame2();
}