#include "TestGame2.hpp"

extern "C" {
	__declspec(dllexport) engine::Game* createGame(engine::Renderer* renderer, engine::Window* window, engine::InputFramework* inputFramework, engine::ResourceManager* resourceManager) {
		engine::Renderer::instance = renderer;
		engine::Window::instance = window;
		engine::InputFramework::instance = inputFramework;
		engine::Logger::init();
		engine::ResourceManager::instance = resourceManager;
		
		engine::Game* game = new engine::TestGame2();
		resourceManager->changeGame(game);
		return game;
	}
}
namespace engine {
	double TestGame2::getTargetFrameRate() {
		return 10;
	}

	TestGame2::TestGame2()
	{
		name = "TestGame2";
		glewInit();
	}

	void TestGame2::update() {

	}

	void TestGame2::initialize() {

		std::shared_ptr<MeshComponent> meshComponent1 = MeshComponent::createMeshFromObjFile("amugus.obj");
		std::weak_ptr<engine::Material> amogusMaterial = createMaterial("AmogusMaterial");
		amogusMaterial.lock()->baseColor = glm::vec3(1.f, 0, 0);
		meshComponent1->setMaterial(amogusMaterial);
		
		std::shared_ptr<MeshComponent> meshComponent2 = MeshComponent::createPrimitive(PrimitiveMeshType::CUBE);

		std::shared_ptr<MeshComponent> platformMesh = MeshComponent::createPrimitive(PrimitiveMeshType::CUBE);
		std::weak_ptr<engine::Material> platformMaterial = createMaterial("PlatformMaterial");
		platformMaterial.lock()->baseColor = glm::vec3(0, 0.5f, 0);
		platformMesh->setMaterial(platformMaterial);

		PhysicsComponent physicsComponentWithGravity = PhysicsComponent(true);
		PhysicsComponent physicsComponentWithoutGravity = PhysicsComponent(false);

		auto boxColliderComponent = BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.5f, 3.f, 1.5f));
		auto groundColliderComponent = BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(10.1f, 0.3f, 10.1f), true);
		auto platformColliderComponent = BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(2.f, 0.1f, 2.f), true);


		GameObject* amogus = new GameObject();
		amogus->transform.setPosition(glm::vec3(0, 1.8f, 0));
		amogus->name = "Player";
		amogus->addComponent(meshComponent1);
		amogus->addComponent(std::make_unique<PhysicsComponent>(physicsComponentWithGravity));
		amogus->addComponent(std::make_unique<BoxColliderComponent>(boxColliderComponent));
		amogus->addComponent(std::make_unique<ControllableComponent>(true, true));
		addGameObject(std::unique_ptr<GameObject>(amogus));


		GameObject* ground = new GameObject();
		ground->transform.setScale(glm::vec3(10.f, 0.2f, 10.f));
		ground->transform.setPosition(glm::vec3(0, 0, 0));
		ground->name = "Ground";
		ground->addComponent(meshComponent2);
		ground->addComponent(std::make_unique<BoxColliderComponent>(groundColliderComponent));
		addGameObject(std::unique_ptr<GameObject>(ground));

		GameObject* platform = new GameObject();
		platform->transform.setScale(glm::vec3(2.f, 0.1f, 2.f));
		platform->transform.setPosition(glm::vec3(2.f, 1.f, 0));
		platform->name = "Platform";
		platform->addComponent(platformMesh);
		platform->addComponent(std::make_unique<BoxColliderComponent>(platformColliderComponent));
		addGameObject(std::unique_ptr<GameObject>(platform));


		GameObject* camera = new GameObject();
		camera->transform.setPosition(glm::vec3(0, 0, 15));
		camera->addComponent(std::make_unique<CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));;

		auto pointLightComponent = std::make_shared<PointLightComponent>();
		pointLightComponent->quadratic = 0;
		pointLightComponent->linear = 0;
		pointLightComponent->constant = 2.5f;
		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->addComponent(pointLightComponent);
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));
	}

}

engine::Game* engine::createGame() {

	return new TestGame2();
}