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
		game->createGameObject("Lua gameobject");
	}

	void ScriptEngine::updateScriptableComponent(ScriptableComponent* component)
	{
		//LOG_INFO("Updating scriptable component");

		sol::function update = component->state["update"];
		update();


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
		
		system("dotnet ../../C#ToLuaCompiler/CSharp.Lua.Launcher.dll -s ../../Scripting -d ../../LuaScripts/CompiledScripts");


		// Require doesn't work if only sol is used, using base lua for loading state and sol for the rest
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		/*
		
		int r = luaL_dofile(L, "../../test.lua");

		if (r != LUA_OK)
		{
			LOG_ERROR("Error: {0}", lua_tostring(L, -1));
		}
		else
		{
			LOG_INFO("Success");

			lua_getglobal(L, "a");

			if (lua_isnumber(L, -1))
			{
				LOG_INFO("Result: {0}", lua_tonumber(L, -1));
			}
		}

		*/
		
				
		sol::state_view lua(L);
		lua.open_libraries(sol::lib::base);
		//bindGameObjectToLueState(component);

	//	lua.load_file("../Debug/out/manifest.lua");
		lua.script_file("../../LuaScripts/launcher.lua");
		lua.load_file("../../LuaScripts/CompiledScripts/test.lua");

		int num = lua["Program"]["hehe"];
		sol::function func = lua["Program"]["AddComponent"];
		std::cout << num << std::endl;
		func();

		//sol::function func = lua["Main"];
		//func();
	//	sol::load_result script2 = component->state.load_file("../../engineAPI.lua");
	//	script1();
		

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

		//lua.open_libraries(sol::lib::base);

		
		
		//lua.set("scriptClass", this);
		//lua["m1"] = &ScriptEngine::addGameObject;
		//lua.set_function("")

		//lua.script_file("../../test.lua");
		
	//	sol::function initialize = lua["initialize"];
		//initialize();

		//sol::function update = lua["update"];
	//	update();




		/*
		

		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		int r = luaL_dofile(L, "../../test.lua");

		if (r != LUA_OK)
		{
			LOG_ERROR("Error: {0}", lua_tostring(L, -1));
		}
		else
		{
			LOG_INFO("Success");

			lua_getglobal(L, "a");

			if (lua_isnumber(L, -1))
			{
				LOG_INFO("Result: {0}", lua_tonumber(L, -1));
			}
		}

		system("pause");
		lua_close(L);
				*/
	}
}
