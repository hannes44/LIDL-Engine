#include <iostream>
#include "TestGame.hpp"
#include "Logger.hpp"
#include <GL/glew.h>
#include "Window.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "MeshComponent.hpp"
#include <memory>

int main(int argc, char* argv[])
{
	engine::Logger::init();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	engine::Window::getInstance().createWindow(800, 600, "Test Window");

	glewInit();

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);
	
	engine::MeshComponent meshComponent = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");
	engine::GameObject gameObject = engine::GameObject();

	std::vector<engine::Vertex> vertices{};
	std::vector<uint32_t> indices{};
	gameObject.components.push_back(std::make_unique<engine::MeshComponent>(meshComponent));

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
		}
		

		engine::Renderer::renderGame(gameObject);


		SDL_GL_SwapWindow(engine::Window::getInstance().window);
	}

	return 0;
}

