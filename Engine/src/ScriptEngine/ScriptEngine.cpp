#include "ScriptEngine.hpp"
#include "Core/Logger.hpp"

namespace engine
{

	extern "C"
	{
		#include "../../vendor/Lua/include/lua.h"
		#include "../../vendor/Lua/include/lauxlib.h"
		#include "../../vendor/Lua/include/lualib.h"
	}

	#pragma comment(lib, "../../vendor/Lua/lua54.lib")

	void ScriptEngine::start()
	{
		LOG_INFO("Starting script engine");

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
	}
}
