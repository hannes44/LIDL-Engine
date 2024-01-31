#include "Editor.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "EditorGui.hpp"
#include "MainMenuGUI.hpp"
#include "EditorSerializer.hpp"
#include "Project.hpp"
#include "TestGame.hpp"

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

		// Comment out this to access the main menu, temporary for development
		project = std::make_shared<Project>();
		project->game = std::make_shared<TestGame>();
		
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

