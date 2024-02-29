#pragma once
#include "Component.hpp"
#include "glm/glm.hpp"


namespace engine
{
	class PhysicsComponent : public Component
	{
	public:
		friend class GamePhysics;
		PhysicsComponent(bool enableGravity = true) : enableGravity(enableGravity) {};

		bool enableGravity = true;
		bool overrideGravityCoefficient = false;

		float gravityCoefficient = 9.82f;
		float mass = 1.f;

		glm::vec3 currentForce{ 0 };
		glm::vec3 currentAcceleration{ 0 };
		glm::vec3 currentVelocity{ 0 };

		glm::vec3 forward = glm::vec3(0, 0, 1);
		glm::vec3 right = glm::vec3(1, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::vec3 getAcceleration() { return currentAcceleration; };
		glm::vec3 getForce() { return currentForce; };
		void applyForce(glm::vec3 force) { currentForce += force; };
		void setForce(glm::vec3 force) { currentForce = force; };

		glm::vec3 getVelocity() { return currentVelocity; };
		void applyVelocity(glm::vec3 velocity) { currentVelocity += velocity; };
		void setVelocity(glm::vec3 velocity) { currentVelocity = velocity; };

		std::string getName() override { return "Physics"; };

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::BOOLEAN, "Gravity", "Should Gravity affect the game object", &enableGravity},
				{SerializableType::FLOAT, "Mass", "The mass of the game object in Kg", &mass},
				{SerializableType::VECTOR3, "Force", "The current force acting on the game object", &currentForce},
				{SerializableType::VECTOR3, "Acceleration", "The current resulting acceleration of the game object", &currentAcceleration},
				{SerializableType::VECTOR3, "Velocity", "The current velocity of the game object", &currentVelocity},
			};
		};
	};
}