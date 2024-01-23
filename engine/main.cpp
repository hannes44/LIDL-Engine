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
	
	engine::MeshComponent meshComponent1 = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");
	engine::MeshComponent meshComponent2 = engine::MeshComponent::loadMeshFromOBJFile("amugus.obj");

	engine::GameObject gameObject1 = engine::GameObject();
	gameObject1.transform.setScale(glm::vec3(5,5,5));

	engine::GameObject gameObject2 = engine::GameObject();
	gameObject2.transform.setScale(glm::vec3(5, 5, 5));
	gameObject2.transform.setPosition(glm::vec3(15, 0, 0));


	engine::Camera camera = engine::Camera();
	camera.translation = glm::vec3(30, 30, 30);
	camera.direction = glm::normalize(glm::vec3(-1, -1, -1));

	gameObject1.components.push_back(std::make_unique<engine::MeshComponent>(meshComponent1));
	gameObject2.components.push_back(std::make_unique<engine::MeshComponent>(meshComponent2));

	engine::Shader* shader = engine::Shader::create("", "simple.vert", "simple.frag");

	engine::Renderer::baseShader = std::unique_ptr<engine::Shader>(shader);

	std::vector<engine::GameObject*> gameObjects{};
	gameObjects.push_back(&gameObject1);
	gameObjects.push_back(&gameObject2);

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
		}
		

	    engine::Renderer::renderGame(gameObjects, camera);


		SDL_GL_SwapWindow(engine::Window::getInstance().window);
	}

	return 0;
}

