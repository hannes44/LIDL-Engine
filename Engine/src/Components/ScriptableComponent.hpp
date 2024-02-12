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

		std::string name = "Scriptable component";


	};
}