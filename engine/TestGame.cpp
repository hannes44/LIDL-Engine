#include <iostream>
#include "Game.hpp"
#include "TestGame.hpp"
#include "Bootstrap.hpp"

namespace engine {
    double TestGame::getTargetFrameRate() {
        return 1;
    }

    void TestGame::update() {
        Display *display = Bootstrap::getInstance().getDisplay();
        display->clear();

        if(frameCount % 2 == 0){
            display->drawText("Frame: " + std::to_string(frameCount), 0, 0, Color::WHITE);
        } else{
            std::string size = std::to_string(display->getWidth()) + "x" + std::to_string(display->getHeight());
            display->drawText("Terminal size: " + size, 0, 1, Color::RED);
        }

        Bootstrap::getInstance().getDisplay()->show();
    }

    void TestGame::initialize() {
    }
}