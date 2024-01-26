#pragma once
#include "Component.hpp"


namespace engine
{
	#define GRAVITY 9.8f
	class PhysicsComponent : public Component
	{
	public:
		float gravity = GRAVITY;
		float mass;

		std::string getName() override { return "Physics"; };
	};
}