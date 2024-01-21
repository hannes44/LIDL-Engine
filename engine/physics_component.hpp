#pragma once
#include "component.hpp"


namespace engine
{
	#define GRAVITY 9.8f
	class PhysicsComponent : public Component
	{
		float gravity = GRAVITY;
		float mass;
	};
}