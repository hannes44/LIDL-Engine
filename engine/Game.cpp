#include "Game.hpp"
#include "GameObjectManager.hpp"
#include <chrono>

namespace engine {
    long long Game::getTime() {
        return std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }

	const void Game::gameLoop() {
        while(running) {
            if(getTargetFrameRate() > 0 && Game::getTime() - lastFrameTime < 1000000000 / getTargetFrameRate()) {
                continue;
            }

            frameCount++;
            lastFrameTime = Game::getTime();
            update();
            GameObjectManager::getInstance().update();
        }
	}
}