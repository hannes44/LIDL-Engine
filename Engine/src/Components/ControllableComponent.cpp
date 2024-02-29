#include "PhysicsComponent.hpp"
#include "ControllableComponent.hpp"
#include "Core/Logger.hpp"
#include "Core/GameObject.hpp"
#include "Core/Game.hpp"
#include "Input/InputFramework.hpp"
#include <iostream>
#include <glm/gtx/transform.hpp>

#include <set>
#include <string>

namespace engine {

	ControllableComponent::ControllableComponent() 
	{
		InputFramework::getInstance().addListener(this);
	}

	void ControllableComponent::initialize() 
	{
		if (gameObject->game)
			enableForces = gameObject->game->config.physicsSettings.enableForces;
	}

	ControllableComponent::~ControllableComponent()
	{
		InputFramework::getInstance().removeListener(this);
	}

	std::set<std::string> ControllableComponent::getRequiredComponents() 
	{
		return { "Physics" };
	}

	std::string ControllableComponent::getName() 
	{
		return "Controllable";
	}

	void ControllableComponent::apply(std::shared_ptr<PhysicsComponent> physicsComponent, glm::vec3 vector) 
	{
		if (enableForces)
			physicsComponent->applyForce(vector);
		else
			physicsComponent->applyVelocity(vector);
	}

	void ControllableComponent::moveOnHold(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent) 
	{
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

	void ControllableComponent::alwaysMove(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent) 
	{
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

	void ControllableComponent::moveOnHoldFromDirection(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent, glm::vec3 direction) 
	{
		if (eventType != InputEventType::KeyDown && eventType != InputEventType::KeyUp)
			return;

		Direction dir = Direction::None;

		switch(event.getKey())
		{
			case Key::LSHIFT: dir = Direction::Up; break;
			case Key::LCTRL: dir = Direction::Down; break;
			case Key::A: dir = Direction::Left; break;
			case Key::D: dir = Direction::Right; break;
			case Key::W: dir = Direction::Forward; break;
			case Key::S: dir = Direction::Backward; break;
			default: return;
		}

		if (eventType == InputEventType::KeyDown)
			currentDirections.insert(dir);
		else
			currentDirections.erase(dir);
	}

	void ControllableComponent::changeDirection(const InputEvent& event)
	{
		InputEventType EventType = event.getEventType();

		// Calculate the new orientation quaternion from the direction vector
		orientation = glm::quatLookAt(direction, worldUp);

		// Handle key and mouse input here
		// If mouse button is pressed we want to control the camera
		if (EventType == InputEventType::MouseButtonDown && (Key)event.getButton() == Key::MOUSE_RIGHT) 
		{
			isMouseDragging = true;
		}

		// If mouse button is released we want to stop controlling the camera
		if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))) 
		{
			isMouseDragging = false;
		}

		if ((EventType == InputEventType::MouseMotion || EventType == InputEventType::KeyDown || EventType == InputEventType::KeyHold)
			&& isMouseDragging) 
		{
			if (SDL_BUTTON(SDL_BUTTON_RIGHT)) 
			{
				// Calculate the pitch and yaw quaternions
				glm::quat pitch = glm::angleAxis(rotationSpeed * -event.getY(), glm::vec3(1, 0, 0));
				glm::quat yaw = glm::angleAxis(rotationSpeed * -event.getX(), glm::vec3(0, 1, 0));

				// Apply the pitch and yaw rotations to the orientation quaternion
				orientation = glm::normalize(yaw * orientation * pitch);				

				// Calculate the new direction vector from the orientation quaternion
				direction = orientation * glm::vec3(0, 0, -1);

				gameObject->transform.setRotationFromQuaternion(orientation);
			}
		}
	}

	void ControllableComponent::handleInput(const InputEvent& event) 
	{
		InputEventType eventType = event.getEventType();
		switch (movementType) {
		case MovementType::OnHold:
			moveOnHoldFromDirection(event, eventType, gameObject->getComponent<PhysicsComponent>(), direction);
			if (gameObject->getComponent<CameraComponent>())
			{
				changeDirection(event);
			}
			break;
		case MovementType::Always:
			alwaysMove(event, eventType, gameObject->getComponent<PhysicsComponent>());
			if (gameObject->getComponent<CameraComponent>())
			{
				changeDirection(event);
			}
			break;
		}
	}
}