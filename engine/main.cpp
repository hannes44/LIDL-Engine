#include <iostream>
#include "TestGame.hpp"
#include "Logger.hpp"
#include <GL/glew.h>
#include "Window.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "MeshComponent.hpp"
#include <memory>
#include "InputFramework.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

int main(int argc, char* argv[])
{
	engine::Logger::init();

	engine::Window::getInstance().createWindow(800, 600, "Sweddes MAMMA PÅ PIZZA");

	glewInit();

	engine::InputFramework::getInstance().initialize();

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);

	engine::Shader* shader = engine::Shader::create("simple.vert", "simple.frag");

	engine::Renderer::baseShader = std::unique_ptr<engine::Shader>(shader);

	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();

	return 0;
}

