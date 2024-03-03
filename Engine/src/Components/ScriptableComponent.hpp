#pragma once
#include "Component.hpp"
#include "ScriptEngine/ScriptEngine.hpp"
#include <sol/sol.hpp>
#include "Events/EventManager.hpp"
#include "Input/InputFramework.hpp"

namespace engine
{
	class ScriptEngine;

	class ScriptableComponent : public Component, public EventListener, public InputListener
	{
	public:
		ScriptableComponent();
		~ScriptableComponent();
		

		std::string getName() override { return name; };

		void update() override;

		void initialize() override;

		void setScriptFileName(std::string scriptFileName);

		bool enableInput = true;

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
			scriptVariables.push_back({ SerializableType::BOOLEAN, "enableInput", "Whether the script should handle input", &enableInput });
			return scriptVariables;
		};

		std::vector<SerializableVariable> serializableVariables { 
			 
		};

		// The scripts serialized variables will sync their values to a void pointer corresponding to a scriptable variable
		std::vector<std::shared_ptr<void>> scriptVariablesData{};

		void onEvent(EventType type, std::string message) override;

		void handleInput(const InputEvent& event) override;

		std::shared_ptr<Component> clone() override {
			std::shared_ptr<ScriptableComponent> clone = std::make_shared<ScriptableComponent>();
			clone->scriptFileName = scriptFileName;
			clone->enableInput = enableInput;
			return clone;
		}
	private:
		std::string scriptFileName = "";
	};
}