#pragma once

#include <memory>
#include <string>

namespace engine
{
	// Component class for Entity-Component-System
	// All components must inherit from this class
	// A future improvement would be to use a component library such as EnTT
	// to improve performance caused by bad memory locality
	class Component
	{
	public:
		virtual void initialize() {};
		virtual void update() {};

		virtual std::string getName() { return ""; };

		virtual std::string getDescription() { return ""; };
		// TODO: store UUID to entity
	};
}
