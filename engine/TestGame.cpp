#include <iostream>
#include "Game.hpp"
#include "GameObject.hpp"
#include "TestGame.hpp"
#include "Bootstrap.hpp"

namespace engine {
	double TestGame::getTargetFrameRate() {
		return 1;
	}

	void TestGame::update() {
		Bootstrap::getInstance().getDisplay().clear();

		for (auto gameObject : gameObjects) {
			Bootstrap::getInstance().getDisplay().drawGameObject(gameObject);
		}

		if (frameCount % 2 == 0) {
			Bootstrap::getInstance().getDisplay().drawText("Frame: " + std::to_string(frameCount), 0, 0, Color::WHITE);
		}
		else {
			std::string size = std::to_string(Bootstrap::getInstance().getDisplay().getWidth()) + "x" + std::to_string(Bootstrap::getInstance().getDisplay().getHeight());
			Bootstrap::getInstance().getDisplay().drawText("Terminal size: " + size, 0, 1, Color::RED);
		}

		Bootstrap::getInstance().getDisplay().show();
	}

	void TestGame::initialize() {
		GameObject* sphere1 = new GameObject();
		sphere1->transform.setScale(glm::vec3(2, 2, 2));
		sphere1->transform.setPosition(glm::vec3(10, 10, 0));
		gameObjects.push_back(sphere1);

		GameObject* sphere2 = new GameObject();
		sphere2->transform.setScale(glm::vec3(1, 1, 1));
		sphere2->transform.setPosition(glm::vec3(15, 10, 0));
		gameObjects.push_back(sphere2);
	}
}