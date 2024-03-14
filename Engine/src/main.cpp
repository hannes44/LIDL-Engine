#include <iostream>
#include "Core/Logger.hpp"
#include "Core/Game.hpp"
#include "Core/Window.hpp"
#include "Renderer/Renderer.hpp"
#include "Input/InputFramework.hpp"
#include "Core/ResourceManager.hpp"
#include "Serializer/GameSerializer.hpp"

// The extern function createGame is defined in the game file
extern engine::Game* engine::createGame();

int main(int argc, char* argv[])
{
	engine::Logger::init();
	engine::LOG_INFO("Initializing the engine");

	engine::Window& window = engine::Window::getInstance();
	window.createWindow(1280, 720, "Game");

	glewInit();

	engine::Renderer* renderer = engine::Renderer::getInstance();

	renderer->initGraphicsAPI(engine::GraphicsAPIType::OpenGL);

	renderer->baseShader = engine::Shader::create("simple.vert", "simple.frag");

	engine::InputFramework& inputFramework = engine::InputFramework::getInstance();

	auto game = engine::createGame();
	window.setWindowTitle(game->name);

	engine::ResourceManager::getInstance()->changeGame(game);

	engine::GameSerializer::deserializeGame(game);

	game->initialize();
	game->gameLoop();

	return 0;
}

