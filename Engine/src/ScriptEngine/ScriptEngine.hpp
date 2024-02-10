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

		void updateScriptableComponent(ScriptableComponent* component);

		void initializeScriptableComponent(ScriptableComponent* component);

		Game* game;

		static ScriptEngine* getInstance();

		static inline ScriptEngine* instance;
	private:
		
		void initializeLuaStateForScriptableComponent(ScriptableComponent* component);

		void bindGameObjectToLueState(ScriptableComponent* component);

		void bindTransformToLuaState(ScriptableComponent* component);

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