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

	ControllableComponent::ControllableComponent(bool allowJump) : allowJump(allowJump)
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

	Direction keyToDirection(Key key) 
	{
		switch (key) 
		{
			case Key::W: return Direction::Forward;
			case Key::S: return Direction::Backward;
			case Key::A: return Direction::Left;
			case Key::D: return Direction::Right;
			case Key::LCTRL: return Direction::Down;
			case Key::LSHIFT: return Direction::Up;
			default: return Direction::None;
		}
	}

	void ControllableComponent::alwaysMove(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent) 
	{
		if (eventType != InputEventType::KeyDown)
			return;

		Direction dir = keyToDirection(event.getKey());

		currentDirections.clear();
		if (dir != Direction::None)
			currentDirections.insert(dir);
	}

	void ControllableComponent::moveOnHold(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent) 
	{
		if (eventType != InputEventType::KeyDown && eventType != InputEventType::KeyUp)
			return;

		Direction dir = keyToDirection(event.getKey());

		if (dir == Direction::None)
			return;

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
			isMouseDragging = true;

		// If mouse button is released we want to stop controlling the camera
		if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))) 
			isMouseDragging = false;

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

		if (allowJump && eventType == InputEventType::KeyDown && event.getKey() == Key::SPACE) 
		{
			std::shared_ptr<PhysicsComponent> physicsComponent = gameObject->getComponent<PhysicsComponent>();
			if (physicsComponent) {
				gameObject->transform.shiftPosition(glm::vec3(0, 0.1f, 0));
				physicsComponent->applyVelocity(glm::vec3(0, 5.f, 0));
			}
				
			return;
		}

		switch (movementType) {
		case MovementType::OnHold:
			moveOnHold(event, eventType, gameObject->getComponent<PhysicsComponent>());
			if (gameObject->getComponent<CameraComponent>())
				changeDirection(event);
			break;
		case MovementType::Always:
			alwaysMove(event, eventType, gameObject->getComponent<PhysicsComponent>());
			if (gameObject->getComponent<CameraComponent>())
				changeDirection(event);
			break;
		}
	}
}