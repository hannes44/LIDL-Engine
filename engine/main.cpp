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

int main(int argc, char* argv[])
{
	engine::Logger::init();

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	engine::Window::getInstance().createWindow(800, 600, "Sweddes MAMMA PÅ PIZZA");

	glewInit();

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);
	
	//engine::MeshComponent meshComponent = engine::MeshComponent::createPrimative(engine::PrimativeMeshType::CUBE);
	engine::MeshComponent meshComponent = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");

	engine::GameObject gameObject = engine::GameObject();

	gameObject.transform.setScale(glm::vec3(5,5,5));

	engine::Camera camera = engine::Camera();
	camera.translation = glm::vec3(15, 15, 15);
	camera.direction = glm::normalize(glm::vec3(-1, -1, -1));

	std::vector<engine::Vertex> vertices{};
	std::vector<uint32_t> indices{};
	gameObject.components.push_back(std::make_unique<engine::MeshComponent>(meshComponent));

	engine::Shader* shader = engine::Shader::create("", "simple.vert", "simple.frag");

	engine::Renderer::baseShader = std::unique_ptr<engine::Shader>(shader);

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
		}
		

	    engine::Renderer::renderGame(gameObject, camera);


		SDL_GL_SwapWindow(engine::Window::getInstance().window);
	}

	return 0;
}

