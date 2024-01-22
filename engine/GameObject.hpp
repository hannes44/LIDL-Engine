#pragma once

#include "Transform.hpp"
#include "Component.hpp"
#include <vector>
#include <memory>

namespace engine
{
	class GameObject
	{
	public:
		GameObject() = default;
		// Called every frame
		virtual void update() = 0;

		// Called at initialization
		virtual void initialize() = 0;

		Transform transform;

		bool isVisible = true;

	private:
		// TODO: Should limit each component to one of each type
		std::vector<std::shared_ptr<Component>> components{};
	};
}
