#include <iostream>
#include "InputFramework.hpp"
#include "Window.hpp"
#include "Game.hpp"
#include "Cube.hpp"
#include "GameObject.hpp"
#include "TestGame.hpp"
#include "Bootstrap.hpp"

namespace engine {
	double TestGame::getTargetFrameRate() {
		return 1;
	}

	void TestGame::update() {
		//InputFramework::getInstance().getInput();
	}

	void TestGame::initialize() {
		// Initialize the InputFramework and InputSystem
		InputFramework::getInstance().initialize();
		InputSystem::initialize();

		Cube cube;
		InputSystem::addListener(&cube);

		Window::getInstance().createWindow(800, 600, "Test Game");
	}
}