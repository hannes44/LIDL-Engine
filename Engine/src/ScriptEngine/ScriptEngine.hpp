#pragma once
#include <iostream>
#include "Core/Game.hpp"
#include "Components/ScriptableComponent.hpp"
#include "sol/sol.hpp"

namespace engine
{
	class ScriptableComponent;

	class ScriptEngine
	{
	public:
		~ScriptEngine() = default;
		ScriptEngine(const ScriptEngine&) = delete;

		// Sets up the lua states and binds the engine API to the lua state
		void loadScriptStatesIntoNewLuaState(Game* game);

		void updateScriptableComponent(ScriptableComponent* component);

		void initializeScriptableComponent(ScriptableComponent* component);

		// Parses the script file and fetches all the serializable variables
		void fetchSerializableVariables(ScriptableComponent* component);

		void checkForUpdatedScripts();

		void recompileScripts();

		void initializeLuaStateForScriptableComponent(ScriptableComponent* component);

		void handleInputForScriptableComponent(ScriptableComponent* component, const InputEvent& event);

		bool isSuccessfullyCompiled = false;

		std::string lastCompilationError = "";

		static ScriptEngine* getInstance();

		static inline ScriptEngine* instance;
	private:
		Game* game;

		void bindEngineAPIToLuaState();

		void syncTransformStateEngineToScript(ScriptableComponent* component);

		void syncTransformStateScriptToEngine(ScriptableComponent* component);

		void syncScriptableVariablesToScript(ScriptableComponent* component);

		void syncScriptableVariablesToEngine(ScriptableComponent* component);

		void updateLauncherScript();

		std::string getVariableNameFromLine(const std::string& line);

		// Returns the data and the type of the variable
		std::tuple<std::shared_ptr<void>, SerializableType>getVariableDataFromLine(const std::string& variableName, ScriptableComponent* component);

		void addSerializableVariableFromLine(const std::string& line, ScriptableComponent* component);

		lua_State* L = luaL_newstate();

		// Since the c++ binding is done with lua files, we leave empty functions in the c# API with the binding lua binding functions
		// commented out. After it is compiled to lua, we remove the commented functinos and the API will now be binded to the c++ functions
		void decodeCompiledAPILuaFiles();

		void compileCSharpFilesToLua();

		// Storing the sizes of the files to determine if they have been updated
		std::unordered_map<std::string, int> scriptFileByteSizes{};

		ScriptEngine() = default;
	};
}