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

	void ScriptEngine::start(Game* game)
	{
		this->game = game;
		LOG_INFO("Starting script engine");

		sol::state lua;
		lua.open_libraries(sol::lib::base);

		lua.set_function("meow", &meow);
		
		
		lua.set("scriptClass", this);
		lua["m1"] = &ScriptEngine::addGameObject;
		//lua.set_function("")

		lua.script_file("../../test.lua");




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
