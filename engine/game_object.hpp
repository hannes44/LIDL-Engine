#pragma once

#include "transform.hpp"

namespace engine
{
	class GameObject
	{
	public:
		// Called every frame
		virtual void update() = 0;

		// Called at initialization
		virtual void initialize() = 0;

		Transform transform;

		bool isVisible = true;

		bool isPhysicsEnabled = false;
	};
}
