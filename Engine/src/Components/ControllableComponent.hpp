#pragma once

#include "Component.hpp"
#include "Input/InputListener.hpp"
#include "ControllableComponent.hpp"
#include "Core/GameObject.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <set>
#include <unordered_set>
#include <string>

namespace engine {

	enum MovementType {
		OnHold,
		Always
	};

	class ControllableComponent : public Component, public InputListener {
	public:
		ControllableComponent();

		float movementSpeed = 3;
		MovementType movementType = MovementType::Always;
		bool allowInstantTurnaround = true;

		std::string getName() override;

		std::set<std::string> getRequiredComponents() override;

		void handleInput(const InputEvent& event) override;
	protected:
		std::unordered_set<glm::vec3> applied{};
		void apply(std::shared_ptr<PhysicsComponent> physicsComponent, glm::vec3 vector);

		void moveOnHold(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent);
		void alwaysMove(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent);
	};
}