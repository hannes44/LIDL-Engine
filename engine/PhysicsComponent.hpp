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

		std::string getName() override { return "Physics"; };
	};
}