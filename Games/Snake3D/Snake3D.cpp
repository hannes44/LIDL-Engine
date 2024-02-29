#include "Snake3D.hpp"
#include <iostream>
#include <sstream>
#include <vector>

extern "C"
{
	__declspec(dllexport) engine::Game* createGame(engine::Renderer* renderer, engine::Window* window, engine::InputFramework* inputFramework, engine::ResourceManager* resourceManager)
	{
		engine::Renderer::instance = renderer;
		engine::Window::instance = window;
		engine::InputFramework::instance = inputFramework;
		engine::Logger::init();
		engine::Game* game = new engine::Snake3D();
		engine::ResourceManager::instance = resourceManager;
		resourceManager->changeGame(game);
		return game;
	}
}
namespace engine
{
	double Snake3D::getTargetFrameRate()
	{
		return 10;
	}

	std::shared_ptr<GameObject> createSnakeBody(short index) {
		GameObject* body = new GameObject();
		body->transform.setPosition(glm::vec3(0, 0, 0));
		std::stringstream ss;
		ss << "Body " << index;
		body->name = ss.str();
		
		body->addComponent(engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE));
		body->addComponent(std::make_unique<engine::BoxColliderComponent>(engine::BoxColliderComponent(glm::vec3(0, 0, 0), 0.7f * glm::vec3(1.f, 1.f, 1.f))));

		return std::shared_ptr<GameObject>(body);
	}

	glm::vec3 Snake3D::getTailDirection() {
		if (snake.size() < 2)
			return glm::normalize(snake[0].lock()->getComponent<engine::PhysicsComponent>()->getVelocity());

		return glm::normalize(snake[snake.size() - 2].lock()->transform.getPosition() - snake.back().lock()->transform.getPosition());
	}

	void Snake3D::addBody() {
		std::shared_ptr<GameObject> newBody = createSnakeBody(snake.size());
		newBody->transform.setPosition(snake.back().lock()->transform.getPosition() - getTailDirection() * (1.f + bodyGap));
		snake.push_back(newBody);
		addGameObject(newBody);
	}

	void Snake3D::moveAppleToRandomPosition() {
		const int bounds = 20;
		const float offset = bounds / 2.f;
		apple.lock()->transform.setPosition(glm::vec3(rand() % bounds - offset, rand() % bounds - offset, rand() % bounds - offset));
	}

	void Snake3D::consumeApple() {
		moveAppleToRandomPosition();
		addBody();
	}

	void Snake3D::handleInput(const InputEvent& event) {
		const InputEventType eventType = event.getEventType();
		if (eventType != InputEventType::KeyDown)
			return;

		// Manual triggers of actions
		if (event.getKey() == Key::SPACE) {
			addBody();
		}
		else if (event.getKey() == Key::RCTRL) {
			consumeApple();
		}
	}

	Snake3D::Snake3D()
	{
		name = "Snake3D";
		glewInit();
		InputFramework::getInstance().addListener(this);
	}

	bool veceql(glm::vec3 a, glm::vec3 b) {
		const float e = 0.1f;
		return abs(a.x - b.x) < e && abs(a.y - b.y) < e && abs(a.z - b.z) < e;
	}

	void Snake3D::update()
	{
		for (int i = snake.size() - 1; i > 0; i--) {
			glm::vec3 offs = (1.f + bodyGap) * -glm::normalize(snake[i - 1].lock()->transform.getPosition() - snake[i].lock()->transform.getPosition());
			snake[i].lock()->transform.setPosition(snake[i - 1].lock()->transform.getPosition() + offs);
		}

		auto physicsComponent = snake.front().lock()->getComponent<engine::PhysicsComponent>();
		glm::vec3 vel = glm::normalize(physicsComponent->getVelocity());

		// TODO: Figure out if the axis is flipped or the model is flipped
		// Hotfix for mirrored rotation during movement on Z-axis
		if (vel.x != 0)
			vel.x *= -1;

		// We do not currently support rotations when moving along the Y-axis, so only rotate when moving along the X or Z axis
		if (glm::length(vel) > 0.1f && !veceql(vel, direction)) {
			direction = glm::normalize(vel);
			glm::vec3 dir = direction;
			
			// TODO: Figure out why this works
			// Hotfix for full revolution rotation causing identical angles for opposite directions
			if (dir.x < 0)
				dir = glm::vec3(1, 0, 0.01f);

			// Figure out if the axis is flipped or the model is flipped
			glm::vec3 normal = glm::vec3(0, 1, 0);
			if (direction == glm::vec3(0, 1, 0))
				normal = glm::vec3(0, 0, 1);
			else if (direction == glm::vec3(0, -1, 0))
				normal = glm::vec3(0, 0, -1);

			snake.front().lock()->transform.setRotationFromDirection(direction, normal);
		}
	}

	void Snake3D::gameOver() {
		std::cout << "Game Over!" << std::endl;

		// Remove all bodies but leave the head
		for (auto it = snake.begin() + 1; it != snake.end();) {
			deleteGameObject(it->lock().get());
			it = snake.erase(it);
		}
	}

	void onHeadCollision(Game* game, GameObject* other, ColliderComponent* otherCollider) {
		Snake3D* snakeGame = dynamic_cast<Snake3D*>(game);
		if (snakeGame == nullptr)
			return;

		if (other == snakeGame->apple.lock().get()) {
			snakeGame->consumeApple();
			return;
		}

		for (auto& body : snakeGame->snake) {
			if (body.lock().get() == other) {
				snakeGame->gameOver();
				return;
			}
		}
	}

	void Snake3D::initialize()
	{
		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();

		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->addComponent(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));

		GameObject* camera = new GameObject();
		camera->transform.setPosition(glm::vec3(3, 0, 25));
		camera->addComponent(std::make_unique<engine::CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));

		GameObject* head = new GameObject();
		head->transform.setPosition(glm::vec3(0, 0, 0));
		head->name = "Head";
		auto headColliderComponent = engine::BoxColliderComponent(glm::vec3(0, 0, 0), glm::vec3(1.f, 1.5f, 1.f));
		headColliderComponent.subscribeToCollision(onHeadCollision);
		auto headPhysicsComponent = engine::PhysicsComponent(false);


		auto headControllableComponent = std::make_shared<engine::ControllableComponent>();
		headControllableComponent->movementSpeed = 10.f;
		headControllableComponent->movementType = MovementType::Always;
		headControllableComponent->allowInstantTurnaround = false;

		std::shared_ptr<MeshComponent> headMesh = engine::MeshComponent::createMeshFromObjFile("amugus.obj");
		std::weak_ptr<engine::Material> headMaterial = createMaterial("AmogusMaterial");
		headMaterial.lock()->baseColor = glm::vec3(2.5f, 0, 0);
		headMesh->setMaterial(headMaterial);

		head->addComponent(headMesh);

		head->addComponent(std::make_unique<engine::PhysicsComponent>(headPhysicsComponent));
		head->addComponent(std::make_unique<engine::BoxColliderComponent>(headColliderComponent));
		head->addComponent(headControllableComponent);

		auto headPtr = std::shared_ptr<GameObject>(head);

		addGameObject(headPtr);

		snake.push_back(headPtr);

		GameObject* applePtr = new GameObject();
		applePtr->name = "Apple";
		applePtr->addComponent(std::make_unique<engine::BoxColliderComponent>(engine::BoxColliderComponent(glm::vec3(0, 0, 0), 6.f * glm::vec3(1.f, 1.f, 1.f))));

		std::shared_ptr<MeshComponent> appleMesh = engine::MeshComponent::createMeshFromObjFile("apple.obj");
		std::weak_ptr<engine::Material> appleMaterial = createMaterial("Apple");
		appleMaterial.lock()->baseColor = glm::vec3(0, 2.5f, 0);
		appleMesh->setMaterial(appleMaterial);
		applePtr->addComponent(appleMesh);

		auto appleSharedPointer = std::shared_ptr<GameObject>(applePtr);

		addGameObject(appleSharedPointer);
		apple = appleSharedPointer;
		moveAppleToRandomPosition();

		// Set custom physics settings for Snake
		config.physicsSettings.enableGravity = false;
		config.physicsSettings.enableFriction = false;
		config.physicsSettings.enableCollisions = true;
		config.physicsSettings.enableForces = false;
		config.physicsSettings.collisionResolveType = "DISABLED";
	}

}

engine::Game* engine::createGame()
{

	return new Snake3D();
}