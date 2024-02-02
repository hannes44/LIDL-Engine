#pragma once
#include "Component.hpp"


namespace engine
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(bool disableGravity = false) : disableGravity(disableGravity) {};
		
		bool disableGravity = false;
		bool overrideGravityCoefficient = false;

		float gravityCoefficient = 9.82f;
		float mass = 0;

		glm::vec3 currentVelocity { 0 };

		glm::vec3 getVelocity() { return currentVelocity; };
		void setVelocity(glm::vec3 velocity) { currentVelocity = velocity; };

		std::string getName() override { return "Physics"; };
	};
}