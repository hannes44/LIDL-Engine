#include "FPSGame.hpp"
#include <iostream>

extern "C" {
	__declspec(dllexport) engine::Game* createGame(engine::Renderer* renderer, engine::Window* window, engine::InputFramework* inputFramework, engine::ResourceManager* resourceManager) {
		engine::Renderer::instance = renderer;
		engine::Window::instance = window;
		engine::InputFramework::instance = inputFramework;
		engine::Logger::init();
		engine::ResourceManager::instance = resourceManager;

		engine::Game* game = new engine::FPSGame();
		resourceManager->changeGame(game);
		return game;
	}
}
namespace engine {
	double FPSGame::getTargetFrameRate() {
		return 10;
	}

	FPSGame::FPSGame()
	{
		name = "FPSGame";
		glewInit();
	}

	void FPSGame::update() {

	}

	void FPSGame::initialize() {

	}

}

engine::Game* engine::createGame() {

	return new FPSGame();
}