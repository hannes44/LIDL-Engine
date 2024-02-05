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

		engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);

		#ifdef SKIP_MAIN_MENU
		project = std::make_shared<Project>();
		project->game = loadGameFromDLL("TestGame");
		#endif

		// Comment out this to access the main menu, temporary for development
		//project = std::make_shared<Project>();

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

		EditorSerializer::createFolder(path + "/" + name);

		EditorSerializer::createFolder(path + "/" + name + "/assets");

		EditorSerializer::createFolder(path + "/" + name + "/scripts");
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

		HMODULE testGame = LoadLibraryExW(wideFileName, nullptr, 0);

		if (testGame)
		{
			LOG_INFO("TestGame.dll loaded");
			typedef engine::Game* (*createGame)();
			createGame createGameFunction = (createGame)GetProcAddress(testGame, "createGame");
			if (createGameFunction)
			{
				LOG_INFO("createGame function found");
				game = std::shared_ptr<Game>(createGameFunction());
			}
			else
			{
				LOG_ERROR("createGame function not found");
			}
		}
		else
		{
			LOG_ERROR("TestGame.dll not loaded");
		}
		return game;
	}
	

}

