#include "Editor.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "EditorGUI.hpp"
#include <GL/glew.h>
#include "InputFramework.hpp"
#include "TestGame.hpp"
#include "Renderer.hpp"

namespace engine
{
	Editor::Editor()
	{
	}
	void Editor::start()
	{

		engine::Logger::init();
		LOG_INFO("Starting Editor");

		Window& window = Window::getInstance();
		window.createWindow(1280, 720, "Editor");

		glewInit();

		engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);

		Game* game = new TestGame();
		EditorGUI gui{};
		gui.game = game;
		game->initialize(); // Temporary for testing, should not be called when serialization works

		engine::Renderer::baseShader = engine::Shader::create("simple.vert", "simple.frag");



		while (true)
		{
			gui.renderNewFrame();
			InputFramework::getInstance().getInput();

			Renderer::renderGame(game);

			gui.endFrame();
			window.newFrame();
		}
	}
}

