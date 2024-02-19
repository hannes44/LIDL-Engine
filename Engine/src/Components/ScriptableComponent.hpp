#pragma once
#include "Component.hpp"
#include "ScriptEngine/ScriptEngine.hpp"
#include <sol/sol.hpp>
#include "Events/EventManager.hpp"

namespace engine
{
	class ScriptEngine;

	class ScriptableComponent : public Component, public EventListener
	{
	public:
		ScriptableComponent();

		std::string getName() override { return name; };

		void update() override;

		void initialize() override;

		void setScriptFileName(std::string scriptFileName);

		std::string getScriptFileName() { return scriptFileName; }


		sol::state state;

		std::string name = "Scriptable";

		// Since initialize can be called multiple times if we are using the editor, we need this flag to make sure that the state is only initialized once
		bool stateIsInitialized = false;

		// Currently assuming that the class name is the same as the file name
		std::string getScriptClassName();

		std::vector<SerializableVariable> getSerializableVariables() 
		{ 
			std::vector<SerializableVariable> scriptVariables = serializableVariables;
			scriptVariables.push_back({ SerializableType::STRING, "scriptFileName", "The file name of the C# or Lua files", &scriptFileName });
			return scriptVariables;
		};

		std::vector<SerializableVariable> serializableVariables { 
			 
		};

		// The scripts serialized variables will sync their values to a void pointer corresponding to a scriptable variable
		std::vector<std::shared_ptr<void>> scriptVariablesData{};

		void onEvent(EventType type, std::string message) override;

	private:
		std::string scriptFileName = "";
	};
}