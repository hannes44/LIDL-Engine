#include <iostream>
#include "Game.hpp"
#include "TestGame.hpp"
#include "Bootstrap.hpp"

namespace engine {
	double TestGame::getTargetFrameRate() {
		return 1;
	}

	void TestGame::update() {
		Bootstrap::getInstance().getDisplay().clear();

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
	}
}