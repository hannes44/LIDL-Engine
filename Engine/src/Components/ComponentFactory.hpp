#pragma once
#include "Component.hpp"

namespace engine
{
	class ComponentFactory 
	{
	public:
		static Component* createComponent(std::string componentName);
	};
}