#pragma once

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
		GAMEOBJECT,
	};

	// Variable that will be serialized and be displayed in the editor
	// The name and discription will be used to display the variable in the editor
	struct SerializableVariable
	{
		SerializableType type;
		std::string name;
		std::string description;
		void* data;
		bool showInEditor = true;
	};

	class Serializable
	{
	public:
		// Override this function in order to serialize variables and show them in the editor
		virtual std::vector<SerializableVariable> getSerializableVariables() { return {}; };
	};
}
