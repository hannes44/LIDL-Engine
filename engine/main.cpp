#include <iostream>
#include "Logger.hpp"
#include "Game.hpp"
#include "Window.hpp"
#include "Renderer/Renderer.hpp"
#include "InputFramework.hpp"

extern engine::Game* engine::createGame();

int main(int argc, char* argv[])
{
	engine::Logger::init();
	engine::LOG_INFO("Initializing the engine");

	engine::Window& window = engine::Window::getInstance();
	window.createWindow(1280, 720, "Game");

	glewInit();

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);

	engine::Renderer::baseShader = engine::Shader::create("simple.vert", "simple.frag");

	engine::InputFramework& inputFramework = engine::InputFramework::getInstance();

	auto game =  engine::createGame();
	window.setWindowTitle(game->name);
	game->initialize();
	game->gameLoop();

	return 0;
}

