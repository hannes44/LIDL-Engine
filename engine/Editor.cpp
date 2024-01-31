#include "Editor.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "EditorGui.hpp"
#include "MainMenuGUI.hpp"
#include "EditorSerializer.hpp"

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

		
		MainMenuGUI mainMenuGui{};
		mainMenuGui.MainMenu();

		EditorGUI editorGui{};
		editorGui.start();
	}

	void Editor::createNewProject(const std::string& name, const std::string& path)
	{
		LOG_INFO("Creating new project: {0} at {1}", name, path);

		EditorSerializer::createFolder(path + "/" + name);
	}
	

}

