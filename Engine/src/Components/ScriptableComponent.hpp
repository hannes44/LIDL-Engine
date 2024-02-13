#pragma once
#include "Component.hpp"
#include "ScriptEngine/ScriptEngine.hpp"
#include <sol/sol.hpp>

namespace engine
{
	class ScriptEngine;

	class ScriptableComponent : public Component
	{
	public:
		std::string getName() override { return name; };

		void update() override;

		void initialize() override;

		std::string scriptFileName = "";

		sol::state state;

		std::string name = "Scriptable";

		// Currently assuming that the class name is the same as the file name
		std::string getScriptClassName();

		std::vector<SerializableVariable> getSerializableVariables() 
		{ 
			return 
			{
			{SerializableType::STRING, "scriptFileName", "The file name of the C# or Lua files", &scriptFileName},
			}; 
		};
	};
}