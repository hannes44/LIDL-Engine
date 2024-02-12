#include "ScriptEngine.hpp"
#include "Core/Logger.hpp"
#include <string>
#include <sol/sol.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include<iostream>
#include<algorithm>
#include <windows.h>
#include <atlstr.h>
#include "Utils/Utils.hpp"

namespace engine
{

	extern "C"
	{
		#include "../../vendor/Lua/include/lua.h"
		#include "../../vendor/Lua/include/lauxlib.h"
		#include "../../vendor/Lua/include/lualib.h"
	}

	#pragma comment(lib, "../../vendor/Lua/lua54.lib")

	void ScriptEngine::addGameObject()
	{
		LOG_INFO("Adding game object");
		game->createGameObject("Very cool game object");
	}

	void ScriptEngine::log()
	{
		LOG_FATAL("Log from LUA FILE");
	}

	void ScriptEngine::updateScriptableComponent(ScriptableComponent* component)
	{
		LOG_INFO("Updating scriptable component");
		sol::state_view lua(component->L);

		syncTransformStateEngineToScript(component);
		lua.script("TestComponent.Update(testComponent)");
		syncTransformStateScriptToEngine(component);
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
		// Require doesn't work if only sol is used, using base lua for loading state and sol for the rest
		luaL_openlibs(component->L);
		
		sol::state_view lua(component->L);
		lua.open_libraries(sol::lib::base);

		bindEngineAPIToLuaState(component);

		lua.script_file("../../Games/TestGame/Scripts/launcher.lua");
		
		lua.script("testComponent = TestComponent()");
		lua["testComponent"]["Id"] = component->uuid.id;

		syncTransformStateEngineToScript(component);

		lua.script("TestComponent.Initialize(testComponent)");

		syncTransformStateScriptToEngine(component);
	}

	void ScriptEngine::bindEngineAPIToLuaState(ScriptableComponent* component)
	{
		sol::state_view lua(component->L);
		lua["__log__"] = &ScriptEngine::log;
	}

	void ScriptEngine::syncTransformStateEngineToScript(ScriptableComponent* component)
	{
		sol::state_view lua(component->L);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				// This is conversion to CSharp Matrix4x4 index format
				std::string indexString = "M" + std::to_string(i + 1) + std::to_string(j + 1);
				lua["testComponent"]["gameObject"]["transform"]["transformMatrix"][indexString] = component->gameObject->transform.transformMatrix[i][j];
			}
		}
	}

	void ScriptEngine::syncTransformStateScriptToEngine(ScriptableComponent* component)
	{
		sol::state_view lua(component->L);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				// This is conversion to CSharp Matrix4x4 index format
				std::string indexString = "M" + std::to_string(i + 1) + std::to_string(j + 1);
				component->gameObject->transform.transformMatrix[i][j] = lua["testComponent"]["gameObject"]["transform"]["transformMatrix"][indexString];
			}
		}
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


	void ScriptEngine::decodeCompiledAPILuaFiles()
	{
		// We need to loop through all API files and remove the comments with the pattern "--(c++_API)" from the compiled lua files
		// This is a hack to get the c++ binding to work with the lua files
		std::ostringstream text;
		std::ifstream in_file("../../Games/TestGame/Scripts/Compiled/API/EngineAPI.lua");

		text << in_file.rdbuf();
		std::string str = text.str();

		std::string apiPattern = "--(c++_API)";

		// Removing all occurences of the pattern
		// This will remove the commented out functions that bind the c++ functions to the lua functions
		std::string::size_type n = apiPattern.length();
		for (std::string::size_type i = str.find(apiPattern);
			i != std::string::npos;
			i = str.find(apiPattern))
			str.erase(i, n);

		std::cout << str << std::endl;
		in_file.close();



		std::ofstream out_file("../../Games/TestGame/Scripts/Compiled/API/EngineAPI.lua");
		out_file << str;
	}

	void ScriptEngine::start(Game* game)
	{
		this->game = game;
		LOG_INFO("Starting script engine");

		LOG_INFO("Compiling C# scripts to lua");

		std::string compileCommand = "dotnet ../../engine/src/ScriptingAPI/C#ToLuaCompiler/CSharp.Lua.Launcher.dll -s ../../Games/TestGame/Scripts -d ../../Games/TestGame/Scripts/Compiled";
		std::wstring widestr = std::wstring(compileCommand.begin(), compileCommand.end());
		const wchar_t* widecstr = widestr.c_str();
		
		CStringA result = Utils::ExecCmd(widecstr);

		std::string resultString = result.GetString();
		std::string errorPattern = "error";
		if (resultString.find(errorPattern) != std::string::npos) {
			LOG_FATAL("C# COMPILE ERROR");
			LOG_ERROR("{}", resultString);
			abort();
		}
		LOG_INFO("{}", resultString);
		LOG_INFO("C# scripts compiled to lua");


		decodeCompiledAPILuaFiles();

		LOG_INFO("Copying compiled scripts to build directory");
		// Copying the compiled scripts to the build directory. 
		// TODO: It should be possible to change the path of the lua launcher script instead
		std::filesystem::copy("../../Games/TestGame/Scripts/Compiled/", "../Debug/", std::filesystem::copy_options::overwrite_existing);
		std::filesystem::copy("../../Games/TestGame/Scripts/Compiled/API/", "../Debug/", std::filesystem::copy_options::overwrite_existing);
		LOG_INFO("Compiled scripts copied to build directory");
	}
}
