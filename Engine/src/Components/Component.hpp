#pragma once

#include <memory>
#include <string>
#include <vector>

namespace engine
{

	enum class SerializableType
	{
		STRING,
		INT,
		FLOAT,
		DOUBLE,
		BOOLEAN,
		VECTOR2,
		VECTOR3,
		VECTOR4,
	};

	// Variable that will be serialized and be displayed in the editor
	// The name and discription will be used to display the variable in the editor
	struct SerializableVariable
	{
		SerializableType type;
		std::string name;
		std::string description;
		void* data;
	};

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

		virtual std::vector<SerializableVariable> getSerializableVariables() { return {}; };
	};
}