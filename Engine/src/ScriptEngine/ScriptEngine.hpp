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
		void start(Game* game);

		void updateScriptableComponent(ScriptableComponent* component);

		void initializeScriptableComponent(ScriptableComponent* component);

		void recompileScripts();

		static ScriptEngine* getInstance();

		static inline ScriptEngine* instance;
	private:
		Game* game;

		void initializeLuaStateForScriptableComponent(ScriptableComponent* component);

		void bindEngineAPIToLuaState();

		void syncTransformStateEngineToScript(ScriptableComponent* component);

		void syncTransformStateScriptToEngine(ScriptableComponent* component);

		void updateLauncherScript();

		lua_State* L = luaL_newstate();

		// Since the c++ binding is done with lua files, we leave empty functions in the c# API with the binding lua binding functions
		// commented out. After it is compiled to lua, we remove the commented functinos and the API will now be binded to the c++ functions
		void decodeCompiledAPILuaFiles();

		void compileCSharpFilesToLua();

		ScriptEngine() = default;
	};
}