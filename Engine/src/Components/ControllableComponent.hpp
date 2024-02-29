#pragma once

#include "Component.hpp"
#include "Input/InputListener.hpp"
#include "ControllableComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Core/GameObject.hpp"
#include <glm/glm.hpp>
#include "Physics/GamePhysicsSettings.hpp"

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
		~ControllableComponent();

		float movementSpeed = 3;
		MovementType movementType = MovementType::OnHold;
		bool allowInstantTurnaround = true;
		bool enableForces = true;

		std::string getName() override;
		void initialize() override;

		bool isMouseDragging;

		glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
		glm::vec3 direction{ -1, -1, -1 };
		glm::quat orientation{ 1, 0, 0, 0 };

		float rotationSpeed = 0.005f;

		std::set<Direction> currentDirections;

		std::set<std::string> getRequiredComponents() override;

		void handleInput(const InputEvent& event) override;

		std::shared_ptr<Component> clone() override {
			return std::make_shared<ControllableComponent>(*this);
		}
		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::BOOLEAN, "Enable Forces", "Should forces affect this game object", &enableForces},
				{SerializableType::FLOAT, "Movement Speed", "The speed at which the game object moves", &movementSpeed},
				{SerializableType::FLOAT, "Rotation Speed", "The speed at which the game object rotates", &rotationSpeed},
				{SerializableType::BOOLEAN, "Allow Instant Turnaround", "Should the game object be able to instantly turn around", &allowInstantTurnaround}
			};
		};
	protected:
		std::unordered_set<glm::vec3> applied{};
		void apply(std::shared_ptr<PhysicsComponent> physicsComponent, glm::vec3 vector);

		void moveOnHold(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent);
		void alwaysMove(const InputEvent& event, const InputEventType& eventType, std::shared_ptr<PhysicsComponent> physicsComponent);
        void changeDirection(const InputEvent &event);
    };
}