#pragma once
#include "Component.hpp"

namespace engine
{
	class ComponentFactory
	{
	public:
		static std::shared_ptr<Component> createComponent(std::string componentName);
	};
}