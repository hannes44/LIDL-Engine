#pragma once

#include "Transform.hpp"
#include "Component.hpp"
#include <vector>
#include <memory>
#include <string>

namespace engine
{
	class GameObject
	{
	public:
		GameObject() = default;
		// Called every frame

		void update();

		// Called at initialization
		void initialize();

		Transform transform;

		bool isVisible = true;

		std::string name = "GameObject";

		// TODO: Should limit each component to one of each type
		std::vector<std::shared_ptr<Component>> components{};
	};
}
