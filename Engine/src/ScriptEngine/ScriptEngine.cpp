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
#include "Core/Debug.hpp"
#include "Core/ResourceManager.hpp"
#include <fstream>
#include <sstream>
#include <regex>

namespace engine
{
	extern "C"
	{
		#include "../../vendor/Lua/include/lua.h"
		#include "../../vendor/Lua/include/lauxlib.h"
		#include "../../vendor/Lua/include/lualib.h"
	}

	#pragma comment(lib, "../../vendor/Lua/lua54.lib")

	void ScriptEngine::updateScriptableComponent(ScriptableComponent* component)
	{
		sol::state_view lua(L);

		syncTransformStateEngineToScript(component);
		lua.script(component->getScriptClassName() + ".Update(" + component->uuid.id + ")");
		syncTransformStateScriptToEngine(component);
	}

	void ScriptEngine::initializeScriptableComponent(ScriptableComponent* component)
	{
		if (!component->stateIsInitialized)
		{
			initializeLuaStateForScriptableComponent(component);
			component->stateIsInitialized = true;
		}

		sol::state_view lua(L);

		syncTransformStateEngineToScript(component);

		lua.script(component->getScriptClassName() + ".Initialize(" + component->uuid.id + ")");

		syncTransformStateScriptToEngine(component);

		LOG_INFO("Initializing scriptable component");
	}

	// Checks all scripts for updates and recompiles them if they have been updated
	// Checking the byte size of the files to determine if they have been updated
	// This is not a perfect solution, but it is good enough for now
	// This does not have to be called every frame
	void ScriptEngine::checkForUpdatedScripts()
	{
		for (auto& scriptName : ResourceManager::getInstance()->getAllCSharpScriptsInActiveGame())
		{
			// If the file doesn't exist in the map, we have to recompile the lua state
			if (scriptFileByteSizes.count(scriptName) == 0)
			{
				loadScriptStatesIntoNewLuaState(game);
				return;
			}	
			int savedByteSize = scriptFileByteSizes[scriptName];

			std::string pathToScript = ResourceManager::getInstance()->getPathToGameResource(scriptName);
			int updatedByteSize = std::filesystem::file_size(pathToScript);

			if (savedByteSize != updatedByteSize)
			{
				loadScriptStatesIntoNewLuaState(game);
				return;
			}
		}
	}

	void ScriptEngine::recompileScripts()
	{
		sol::state_view lua(L);

		compileCSharpFilesToLua();

		updateLauncherScript();

		// The launcher script is responsible for loading all other scripts into the lua state
		std::string pathToLauncherScript = ResourceManager::getInstance()->getPathToGameResource("launcher.lua");

		// Launcher script loads all other component scripts into the lua state
		lua.script_file(pathToLauncherScript);
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
		sol::state_view lua(L);

		// The id will be used as variable names for the different components
		std::string Id = component->uuid.id;

		// Initializing the component in the lua state
		lua.script(Id + " = " + component->getScriptClassName() + "()");
		lua[Id]["Id"] = component->uuid.id;
	}

	void ScriptEngine::bindEngineAPIToLuaState()
	{
		sol::state_view lua(L);
		lua["__log__"] = Debug::Log;
		lua.set_function("__addGameObject__", &Game::createGameObject, game);
	}

	// Syncs the transform state of the components gameObject to the lua state
	void ScriptEngine::syncTransformStateEngineToScript(ScriptableComponent* component)
	{
		sol::state_view lua(L);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				// This is conversion to CSharp Matrix4x4 index format
				std::string indexString = "M" + std::to_string(i + 1) + std::to_string(j + 1);
				lua[component->uuid.id]["gameObject"]["transform"]["transformMatrix"][indexString] = component->gameObject->transform.transformMatrix[i][j];
			}
		}
	}

	void ScriptEngine::syncTransformStateScriptToEngine(ScriptableComponent* component)
	{
		sol::state_view lua(L);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				// This is conversion to CSharp Matrix4x4 index format
				std::string indexString = "M" + std::to_string(i + 1) + std::to_string(j + 1);
				component->gameObject->transform.transformMatrix[i][j] = lua[component->uuid.id]["gameObject"]["transform"]["transformMatrix"][indexString];
			}
		}
	}

	// Updates the launcher script with the names of all the scripts in the game
	void ScriptEngine::updateLauncherScript()
	{
		std::vector<std::string> fileNames = ResourceManager::getInstance()->getAllCSharpScriptsInActiveGame();
		std::string fileNamesString = "";
		for (const auto& fileName : fileNames)
		{
			std::string fileNameNoExtension = fileName.substr(0, fileName.find_last_of("."));
			fileNamesString += "\"" + fileNameNoExtension + "\"" + ",";
		}

		// The launcher script is responsible for loading all other scripts into the lua state
		std::string pathToLauncherScript = ResourceManager::getInstance()->getPathToGameResource("launcher.lua");
		std::string sourcePath = ResourceManager::getPathToEditorResource("launcherTemplate.lua");

		std::filesystem::copy(sourcePath, pathToLauncherScript, std::filesystem::copy_options::overwrite_existing);


		std::ostringstream text;
		std::ifstream in_file(pathToLauncherScript);

		text << in_file.rdbuf();
		std::string str = text.str();

		std::string fileNamesPattern = "--FileNames--";
		std::string const modifiedScript = std::regex_replace(str, std::regex(fileNamesPattern), fileNamesString);

		in_file.close();

		std::ofstream out_file(pathToLauncherScript);
		out_file << modifiedScript;
		out_file.close();

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

		in_file.close();

		std::ofstream out_file("../../Games/TestGame/Scripts/Compiled/API/EngineAPI.lua");
		out_file << str;
	}

	void ScriptEngine::compileCSharpFilesToLua()
	{
		LOG_INFO("Compiling C# scripts to lua");

		std::string compileCommand = "dotnet ../../engine/src/ScriptingAPI/C#ToLuaCompiler/CSharp.Lua.Launcher.dll -s ../../Games/TestGame/Scripts -d ../../Games/TestGame/Scripts/Compiled";
		std::wstring widestr = std::wstring(compileCommand.begin(), compileCommand.end());
		const wchar_t* widecstr = widestr.c_str();

		CStringA result = Utils::ExecCmd(widecstr);

		std::string resultString = result.GetString();
		std::string errorPattern = "error";
		if (resultString.find(errorPattern) != std::string::npos) {
			LOG_ERROR("C# COMPILE ERROR");
			LOG_ERROR("{}", resultString);
			isSuccessfullyCompiled = false;
			lastCompilationError = resultString;
			return;
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

		isSuccessfullyCompiled = true;
		lastCompilationError = "";
	}

	// This will remove the existing lua state and create a new. All scripts will be recompiled and loaded into the new lua state
	// If changes are done in the scripts, this function needs to be called to update the lua state
	void ScriptEngine::loadScriptStatesIntoNewLuaState(Game* game)
	{
		LOG_INFO("ScriptEngine: Loading C# script state into lua state");
		L = luaL_newstate();
		sol::state_view lua(L);
		lua.open_libraries(sol::lib::base);

		this->game = game;


		bindEngineAPIToLuaState();

		compileCSharpFilesToLua();
		
		// Require doesn't work if only sol is used, using base lua for loading state and sol for the rest
		luaL_openlibs(L);

		updateLauncherScript();

		std::string pathToLauncherScript = ResourceManager::getInstance()->getPathToGameResource("launcher.lua");

		// Launcher script loads all other component scripts into the lua state
		lua.script_file(pathToLauncherScript);

		EventManager::getInstance().notify(EventType::ScriptsRecompiled, "");

		// Storing the sizes of the files to determine if they have been updated
		scriptFileByteSizes.clear();
		for (auto& scriptName : ResourceManager::getInstance()->getAllCSharpScriptsInActiveGame())
		{
			std::string pathToScript = ResourceManager::getInstance()->getPathToGameResource(scriptName);
			int byteSize = std::filesystem::file_size(pathToScript);
			scriptFileByteSizes[scriptName] = byteSize;
		}

		LOG_INFO("ScriptEngine: C# script state loaded into lua state");
	}
}
