#include "ScriptEngine.hpp"
#include "Core/Logger.hpp"
#include <string>
#include <sol/sol.hpp>


namespace engine
{

	extern "C"
	{
		#include "../../vendor/Lua/include/lua.h"
		#include "../../vendor/Lua/include/lauxlib.h"
		#include "../../vendor/Lua/include/lualib.h"
	}

	#pragma comment(lib, "../../vendor/Lua/lua54.lib")

	struct vars {
		int boop = 0;
	};

	void meow()
	{
		LOG_INFO("Meow");
	}

	void ScriptEngine::addGameObject()
	{
		LOG_INFO("Adding game object");
		game->createGameObject("Very cool game object");
	}

	void ScriptEngine::updateScriptableComponent(ScriptableComponent* component)
	{
		LOG_INFO("Updating scriptable component");
		sol::state_view lua(L);
		sol::function Updatefunc = lua["testComponent"]["Update"];
		Updatefunc();
	}

	void ScriptEngine::initializeScriptableComponent(ScriptableComponent* component)
	{
		initializeLuaStateForScriptableComponent(component);

		LOG_INFO("Initializing scriptable component");
	}

	ScriptEngine* ScriptEngine::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new ScriptEngine();
		}
		return instance;
	}

	void ScriptEngine::initializeLuaStateForScriptableComponent(ScriptableComponent* component)
	{
		
		system("dotnet ../../engine/src/ScriptingAPI/C#ToLuaCompiler/CSharp.Lua.Launcher.dll -s ../../engine/src/ScriptingAPI -d ../../engine/src/ScriptingAPI/Compiled");


		// Require doesn't work if only sol is used, using base lua for loading state and sol for the rest
		luaL_openlibs(L);
		
				
		sol::state_view lua(L);
		lua.open_libraries(sol::lib::base);
		//bindGameObjectToLueState(component);

		lua.script_file("../../engine/src/ScriptingAPI/launcher.lua");

		//sol::usertype<GameObject> gameObjectType = lua.new_usertype<GameObject>("_GameObject", sol::constructors<GameObject()>());
		//gameObjectType["name"] = &GameObject::name;
		//lua["gameobjects"] = std::vector<GameObject*>{ component->gameObject, component->gameObject  };

		lua.script("testComponent = TestComponent.create()");
		//std::cout << "Name: " << component->gameObject->name << std::endl;
		//	lua["name"] = component->gameObject->name;
		sol::function initializefunc = lua["testComponent"]["Initialize"];
		initializefunc();

		std::cout << "Name: " << component->gameObject->name << std::endl;
		
		

	}

	void ScriptEngine::bindGameObjectToLueState(ScriptableComponent* component)
	{
		component->state["gameObject"] = component->gameObject;

		sol::usertype<GameObject> gameObjectType = component->state.new_usertype<GameObject>("_GameObject", sol::constructors<GameObject()>());
		gameObjectType["name"] = &GameObject::name;

		bindTransformToLuaState(component);
	}

	void ScriptEngine::bindTransformToLuaState(ScriptableComponent* component)
	{
		component->state["transform"] = &component->gameObject->transform;

		sol::usertype<glm::vec3> vec3Type = component->state.new_usertype<glm::vec3>("_vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>());
		vec3Type["x"] = &glm::vec3::x;
		vec3Type["y"] = &glm::vec3::y;
		vec3Type["z"] = &glm::vec3::z;


		sol::usertype<Transform> transformType = component->state.new_usertype<Transform>("_Transform", sol::constructors<Transform()>());
		transformType["getPosition"] = &Transform::getPosition;
		transformType["setPosition"] = &Transform::setPosition;
		transformType["shiftPosition"] = &Transform::shiftPosition;
		transformType["Test"] = &Transform::Test;
	}

	void ScriptEngine::start(Game* game)
	{
		this->game = game;
		LOG_INFO("Starting script engine");


	}
}
