#include "MultiplayerGame.hpp"
#include "MultiplayerClient/Client.hpp"
#include "Serializer/GameSerializer.hpp"
#include "Core/Logger.hpp"
#include "Utils/Utils.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <fstream>

extern "C"
{
	__declspec(dllexport) engine::Game* createGame(engine::Renderer* renderer, engine::Window* window, engine::InputFramework* inputFramework, engine::ResourceManager* resourceManager)
	{
		engine::Renderer::instance = renderer;
		engine::Window::instance = window;
		engine::InputFramework::instance = inputFramework;
		engine::Logger::init();
		engine::Game* game = new engine::MultiplayerGame();
		engine::ResourceManager::instance = resourceManager;
		resourceManager->changeGame(game);
		return game;
	}
}
namespace engine
{
	double MultiplayerGame::getTargetFrameRate()
	{
		return 10;
	}

	bool MultiplayerGame::isMultiplayerGame()
	{
		return true;
	}

	void MultiplayerGame::onMultiplayerStateReceived(std::string state)
	{
		std::string filePath = MULTIPLAYER_STATE_FOLDER + "ParsedState" + MULTIPLAYER_STATE_FILE_EXTENSION;

		std::ofstream outfile(filePath);
		outfile << state.c_str();
		outfile.close();

		GameSerializer::deserializeGameState(this, filePath);
	}

	// TODO_MULTIPLAYER: Move this to Game.hpp once include issues are resolved
	void MultiplayerGame::setupMultiplayer() {
		if (!isMultiplayerGame())
			return;

		// Connect to the server
		multiplayerSocket = engine::Client::OpenSocket();

		// Start the multiplayer receiver in a detached thread
		std::thread receiver(engine::Client::RunReceiver, multiplayerSocket, std::bind(&MultiplayerGame::onMultiplayerStateReceived, this, std::placeholders::_1));
		// Start the multiplayer transmitter in a detached thread
		std::thread transmitter(engine::Client::RunTransmitter, multiplayerSocket);

		receiver.detach();
		transmitter.detach();
	}

	// TODO_MULTIPLAYER: Move this to the Physics Engine once include issues are resolved
	void MultiplayerGame::sendMultiplayerState() {
		std::string filePath = GameSerializer::serializeGameState(MULTIPLAYER_STATE_FOLDER, this, true);

		if (!std::ifstream(filePath).good()) {
			LOG_ERROR("File not found: " + filePath);
			return;
		}

		std::ostringstream text;
		std::ifstream in_file(filePath);

		text << in_file.rdbuf();
		std::string message = text.str();
		Client::QueueMessage({ ClientMessageType::StateUpdate, message });
	}

	GameObject* MultiplayerGame::getRemoteBox() {
		for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			auto gameObject = it->second.get();
			if (gameObject->name == "Remote box") {
				return gameObject;
			}
		}
	}

	void MultiplayerGame::moveRemoteBox() {
		auto remoteBox = getRemoteBox();
		remoteBox->transform.shiftPosition(glm::vec3(-2, 0, 0));
	}

	void MultiplayerGame::cycleRemoteBoxColour() {
		auto remoteBox = getRemoteBox();
		auto material = remoteBox->getComponent<MeshComponent>()->getMaterial();
		float greenValue = material->baseColor.g + 1.f;
		if(greenValue > 5.f)
			greenValue = 1.f;

		material->setBaseColor(glm::vec3(0, greenValue, 0));
	}

	void MultiplayerGame::handleInput(const InputEvent& event) {
		const InputEventType eventType = event.getEventType();
		if (eventType != InputEventType::KeyDown)
			return;

		// Manual triggers of actions
		if (event.getKey() == Key::M) {
			moveRemoteBox();
		}
		else if (event.getKey() == Key::C) {
			cycleRemoteBoxColour();
		}
	}

	MultiplayerGame::MultiplayerGame()
	{
		name = "MultiplayerGame";
		glewInit();
		InputFramework::getInstance().addListener(this);
	}

	void MultiplayerGame::update()
	{
		// TODO_MULTIPLAYER: Move this to the update on the Physics Engine once include issues are resolved
		if (Utils::getTimestampNS() - lastMultiplayerStateUpdateTimestamp < 1000000 * multiplayerStateUpdateIntervalMS) {
			return;
		}
		
		lastMultiplayerStateUpdateTimestamp = Utils::getTimestampNS();
		sendMultiplayerState();
	}

	void MultiplayerGame::initialize()
	{
		// TODO_MULTIPLAYER: Move this to the Game initialization once include issues are resolved
		setupMultiplayer();

		engine::PointLightComponent pointLightComponent = engine::PointLightComponent();

		std::shared_ptr<MeshComponent> localMesh = engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE);
		std::shared_ptr<MeshComponent> remoteMesh = engine::MeshComponent::createPrimative(PrimativeMeshType::CUBE);

		engine::PhysicsComponent localPhysicsComponent = engine::PhysicsComponent(false);
		localPhysicsComponent.setForce(glm::vec3(0, 0, 0));

		engine::PhysicsComponent remotePhysicsComponent = engine::PhysicsComponent(false);
		remotePhysicsComponent.setForce(glm::vec3(0, 0, 0));

		GameObject* light = new GameObject();
		light->transform.setPosition(glm::vec3(0, 20, 0));
		light->addComponent(std::make_unique<engine::PointLightComponent>(pointLightComponent));
		light->name = "Light";
		addGameObject(std::unique_ptr<GameObject>(light));

		GameObject* camera = new GameObject();
		camera->transform.setPosition(glm::vec3(0, 10, -30));
		camera->transform.setRotationFromDirection(glm::vec3(0, 0.5f, -1), glm::vec3(0, 1, 0));
		camera->addComponent(std::make_unique<engine::CameraComponent>());
		camera->name = "Camera";
		addGameObject(std::unique_ptr<GameObject>(camera));


		GameObject* localBoxPtr = new GameObject();
		localBoxPtr->transform.setPosition(glm::vec3(5, 0, 0));
		localBoxPtr->name = "Local box";
		localBoxPtr->addComponent(localMesh);
		localBoxPtr->addComponent(std::make_unique<engine::PhysicsComponent>(localPhysicsComponent));

		std::weak_ptr<engine::Material> remoteMaterial = createMaterial("RemoteMaterial");
		remoteMaterial.lock()->baseColor = glm::vec3(0, 2.5f, 0);
		remoteMesh->setMaterial(remoteMaterial);

		GameObject* remoteBoxPtr = new GameObject();
		remoteBoxPtr->transform.setPosition(glm::vec3(0, 0, 0));
		remoteBoxPtr->name = "Remote box";
		remoteBoxPtr->addComponent(remoteMesh);
		remoteBoxPtr->addComponent(std::make_unique<engine::PhysicsComponent>(remotePhysicsComponent));
		remoteBoxPtr->addComponent(std::make_unique<engine::MultiplayerComponent>());
		remoteBoxPtr->addComponent(std::make_unique<engine::ControllableComponent>(false));


		auto localBoxSharedPointer = std::shared_ptr<GameObject>(localBoxPtr);
		auto remoteBoxSharedPointer = std::shared_ptr<GameObject>(remoteBoxPtr);

		addGameObject(localBoxSharedPointer);
		addGameObject(remoteBoxSharedPointer);

		// Set custom physics settings for MultiplayerGame
		config.physicsSettings.enableGravity = false;
		config.physicsSettings.enableFriction = false;
		config.physicsSettings.enableCollisions = false;
		config.physicsSettings.enableForces = false;
		config.physicsSettings.collisionResolveType = "DISABLED";
	}

}

engine::Game* engine::createGame()
{

	return new MultiplayerGame();
}