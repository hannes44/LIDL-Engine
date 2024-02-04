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
	void engine::Editor::start()
	{
		engine::Logger::init();
		LOG_INFO("Starting Editor");

		Window& window = Window::getInstance();
		window.createWindow(1280, 720, "Editor");

		glewInit();

		engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);


		HMODULE testGame = LoadLibraryExW(L"gamelib.dll", nullptr, 0);

		if (testGame)
		{
			LOG_INFO("TestGame.dll loaded");
			typedef engine::Game* (*createGame)();
			createGame createGameFunction = (createGame)GetProcAddress(testGame, "createGame");
			if (createGameFunction)
			{
				LOG_INFO("createGame function found");
				project = std::make_shared<Project>();
				project->game = std::shared_ptr<Game>(createGameFunction());
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
		
		std::cout << project->game->name << std::endl;
		project->game->update();
		return;

		// Comment out this to access the main menu, temporary for development
		project = std::make_shared<Project>();

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
	

}

