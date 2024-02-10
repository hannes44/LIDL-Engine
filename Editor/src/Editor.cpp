#include "Editor.hpp"
#include <Engine.hpp>
#include "Renderer/Renderer.hpp"
#include "GUI/EditorGui.hpp"
#include "GUI/MainMenuGUI.hpp"
#include "Serializer/EditorSerializer.hpp"
#include "Project.hpp"
#include <Windows.h>
#include <iostream>

namespace engine
{
	// Comment out to not skip the main menu
	#define SKIP_MAIN_MENU



	void engine::Editor::start()
	{
		engine::Logger::init();

		LOG_INFO("Starting Editor");

		Window& window = Window::getInstance();
		window.createWindow(1280, 720, "Editor");

		glewInit();
		Renderer* renderer = Renderer::getInstance();
		Renderer::getInstance()->initGraphicsAPI(engine::GraphicsAPIType::OpenGL);
		renderer->baseShader = engine::Shader::create("simple.vert", "simple.frag");

		#ifdef SKIP_MAIN_MENU
		project = std::make_shared<Project>();
		project->game = loadGameFromDLL("SnakeGame");
		#endif

		ScriptEngine scriptEngine{};
		scriptEngine.start(project->game.get());



		if (!project)
		{
			MainMenuGUI mainMenuGui{};
			mainMenuGui.MainMenu();

		}

		EditorGUI editorGui{project};
		editorGui.start();
	}

	void Editor::createNewProject(const std::string& name, const std::string& path)
	{
		LOG_INFO("Creating new project: {0} at {1}", name, path);

		ResourceManager::createFolder(path + "/" + name);

		ResourceManager::createFolder(path + "/" + name + "/assets");

		ResourceManager::createFolder(path + "/" + name + "/scripts");
	}

	void Editor::openProject(const std::string& gameName)
	{
		LOG_INFO("Opening project: {0}", gameName);
		project = std::make_shared<Project>();
		project->game = loadGameFromDLL(gameName);
	}

	std::shared_ptr<Game> Editor::loadGameFromDLL(const std::string gameName)
	{
		std::shared_ptr<Game> game = nullptr;
		std::string fileName = gameName + ".dll";

		// Convert std::string to LPCWSTR
		std::wstring temp = std::wstring(fileName.begin(), fileName.end());
		LPCWSTR wideFileName = temp.c_str();

		HMODULE gameDLL = LoadLibraryExW(wideFileName, nullptr, 0);

		if (gameDLL)
		{
			LOG_INFO("Loaded: {0}", fileName);
			typedef engine::Game* (*createGame)(Renderer*, Window*, InputFramework*, ResourceManager*);
			createGame createGameFunction = (createGame)GetProcAddress(gameDLL, "createGame");
			if (createGameFunction)
			{
				LOG_TRACE("createGame function found");
				game = std::shared_ptr<Game>(createGameFunction(Renderer::getInstance(), &Window::getInstance(), &InputFramework::getInstance(), ResourceManager::getInstance()));
			}
			else
			{
				LOG_ERROR("createGame function not found");
			}
		}
		else
		{
			LOG_ERROR("Failed to load: {0}", fileName);
		}
		return game;
	}
	

}

