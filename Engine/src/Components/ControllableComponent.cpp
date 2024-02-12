#include "PhysicsComponent.hpp"
#include "ControllableComponent.hpp"
#include "Core/Logger.hpp"
#include "Core/GameObject.hpp"
#include "Core/Game.hpp"
#include "Input/InputFramework.hpp"
#include <iostream>

#include <set>
#include <string>

namespace engine {

	ControllableComponent::ControllableComponent() {
		InputFramework::getInstance().addListener(this);
	}

	void ControllableComponent::initialize() {
		if (gameObject->game)
			enableForces = gameObject->game->config.physicsSettings.enableForces;
	}

	std::set<std::string> ControllableComponent::getRequiredComponents() {
		return { "Physics" };
	}

	std::string ControllableComponent::getName() {
		return "Controllable";
	}

	void ControllableComponent::apply(std::shared_ptr<PhysicsComponent> physicsComponent, glm::vec3 vector) {
		if (enableForces)
			physicsComponent->applyForce(vector);
		else
			physicsComponent->applyVelocity(vector);
	}

	void ControllableComponent::moveOnHold(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent) {
		int moveDir = 0;
		if (eventType == InputEventType::KeyDown)
			moveDir = 1;
		else if (eventType == InputEventType::KeyUp)
			moveDir = -1;
		else
			return;

		glm::vec3 vector = glm::vec3(0, 0, 0);

		if (event.getKey() == Key::LSHIFT) {
			vector = glm::vec3(0, moveDir * movementSpeed, 0);
		}

		else if (event.getKey() == Key::LCTRL) {
			vector = glm::vec3(0, -moveDir * movementSpeed, 0);
		}

		else if (event.getKey() == Key::A) {
			vector = glm::vec3(-moveDir * movementSpeed, 0, 0);
		}

		else if (event.getKey() == Key::D) {
			vector = glm::vec3(moveDir * movementSpeed, 0, 0);
		}

		else if (event.getKey() == Key::W) {
			vector = glm::vec3(0, 0, -moveDir * movementSpeed);
		}

		else if (event.getKey() == Key::S) {
			vector = glm::vec3(0, 0, moveDir * movementSpeed);
		}

		else {
			return;
		}

		if (!allowInstantTurnaround && applied.find(-vector) != applied.end())
			return;

		applied.erase(-vector);
		applied.insert(vector);
		apply(physicsComponent, vector);
	}

	void ControllableComponent::alwaysMove(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent) {
		// Only change direction on key down
		if (eventType != InputEventType::KeyDown)
			return;

		glm::vec3 vector = glm::vec3(0, 0, 0);

		if (event.getKey() == Key::LSHIFT) {
			vector = glm::vec3(0, movementSpeed, 0);
		}

		else if (event.getKey() == Key::LCTRL) {
			vector = glm::vec3(0, -movementSpeed, 0);
		}

		else if (event.getKey() == Key::A) {
			vector = glm::vec3(-movementSpeed, 0, 0);
		}

		else if (event.getKey() == Key::D) {
			vector = glm::vec3(movementSpeed, 0, 0);
		}

		else if (event.getKey() == Key::W) {
			vector = glm::vec3(0, 0, -movementSpeed);
		}

		else if (event.getKey() == Key::S) {
			vector = glm::vec3(0, 0, movementSpeed);
		}

		else {
			return;
		}

		if (!allowInstantTurnaround && applied.find(-vector) != applied.end())
			return;

		for (auto it = applied.begin(); it != applied.end();) {
			apply(physicsComponent, -*it);
			applied.erase(it++);
		}

		applied.insert(vector);
		apply(physicsComponent, vector);
	}

	void ControllableComponent::handleInput(const InputEvent& event) {
		InputEventType eventType = event.getEventType();
		switch (movementType) {
		case MovementType::OnHold:
			moveOnHold(event, eventType, gameObject->getComponent<PhysicsComponent>());
			break;
		case MovementType::Always:
			alwaysMove(event, eventType, gameObject->getComponent<PhysicsComponent>());
			break;
		}
	}
}