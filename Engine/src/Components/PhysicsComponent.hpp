#pragma once
#include "Component.hpp"
#include "glm/glm.hpp"


namespace engine
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(bool enableGravity = true) : enableGravity(enableGravity) {};
		
		bool enableGravity = true;
		bool overrideGravityCoefficient = false;

		float gravityCoefficient = 9.82f;
		float mass = 0;

		glm::vec3 currentVelocity { 0 };

		glm::vec3 getVelocity() { return currentVelocity; };
		void setVelocity(glm::vec3 velocity) { currentVelocity = velocity; };

		std::string getName() override { return "Physics"; };

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::BOOLEAN, "Gravity", "Should Gravity affect the game object", &enableGravity},
				{SerializableType::FLOAT, "Mass", "The mass of the game object in Kg", &mass}
			};
		};
	};
}