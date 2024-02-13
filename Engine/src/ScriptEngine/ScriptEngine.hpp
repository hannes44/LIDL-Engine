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

		void start(Game* game);

		void addGameObject();

		static void log(std::string message);

		void updateScriptableComponent(ScriptableComponent* component);

		void initializeScriptableComponent(ScriptableComponent* component);

		Game* game;

		static ScriptEngine* getInstance();

		static inline ScriptEngine* instance;
	private:
		
		void compileCSharpFiles();

		void initializeLuaStateForScriptableComponent(ScriptableComponent* component);

		void bindGameObjectToLueState(ScriptableComponent* component);

		void bindTransformToLuaState(ScriptableComponent* component);

		void bindEngineAPIToLuaState();

		void syncTransformStateEngineToScript(ScriptableComponent* component);

		void syncTransformStateScriptToEngine(ScriptableComponent* component);

		lua_State* L = luaL_newstate();

		// Since the c++ binding is done with lua files, we leave empty functions in the c# API with the binding lua binding functions
		// commented out. After it is compiled to lua, we remove the commented functinos and the API will now be binded to the c++ functions
		void decodeCompiledAPILuaFiles();

		ScriptEngine() = default;

	};

	class Cat
	{
	public:
		Cat() {};
		virtual ~Cat() {};
	public:
		void meow() { std::cout << "MEOWW" << std::endl; };
	};
}