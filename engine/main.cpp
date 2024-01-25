#include <iostream>
#include "TestGame.hpp"
#include "Logger.hpp"
#include <GL/glew.h>
#include "Window.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "MeshComponent.hpp"
#include <memory>
#include "Shader.hpp"
#include "Camera.hpp"
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "GameSerializer.hpp"

int main(int argc, char* argv[])
{
	engine::Logger::init();

	engine::Window::getInstance().createWindow(800, 600, "Sweddes MAMMA PÅ PIZZA");

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);
	
	glewInit();

	auto game = engine::TestGame();
	engine::GameSerializer::SerializeGame(game);
	/*


	engine::Shader* shader = engine::Shader::create("simple.vert", "simple.frag");

	engine::Renderer::baseShader = std::unique_ptr<engine::Shader>(shader);


	game.initialize();
	game.gameLoop();
			*/

	return 0;
}

