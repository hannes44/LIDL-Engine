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

int main(int argc, char* argv[])
{
	engine::Logger::init();

	engine::Window::getInstance().createWindow(800, 600, "Sweddes MAMMA PÅ PIZZA");

	glewInit();

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);

	engine::Shader* shader = engine::Shader::create("", "simple.vert", "simple.frag");

	engine::Renderer::baseShader = std::unique_ptr<engine::Shader>(shader);

	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	
	float mouseSensitivity = 3.f;

	auto startTime = std::chrono::system_clock::now();
	float deltaTime = 0;
	float currentTime = 0;

	int oldX = 0;
	int oldY = 0;

	bool stopGame = false;

	/*

	while (!stopGame)
	{
		std::chrono::duration<float> timeSinceStart = std::chrono::system_clock::now() - startTime;
		deltaTime = timeSinceStart.count() - currentTime;
		currentTime = timeSinceStart.count();



		// Temporarily doing camera motion here until the input manager is done
		// Should be moved to the camera class then
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				stopGame = true;
				engine::Window::getInstance().closeWindow();
			}

			if (event.type == SDL_EVENT_MOUSE_MOTION)
			{
				int deltaX = event.motion.xrel;
				int deltaY = event.motion.yrel;

				glm::vec3 worldUp = glm::vec3(0, 1, 0);

				glm::mat4 yaw = glm::rotate(mouseSensitivity * -deltaX * deltaTime, worldUp);
				glm::mat4 pitch = glm::rotate(mouseSensitivity * -deltaY * deltaTime, glm::normalize(glm::cross(camera.direction, worldUp)));
				camera.direction = glm::vec3(pitch * yaw * glm::vec4(camera.direction, 0.0f));

				oldX = event.motion.x;
				oldY = event.motion.y;
			}
		}
		

	    engine::Renderer::renderGame(gameObjects, camera);


		SDL_GL_SwapWindow(engine::Window::getInstance().window);
	}
		*/
	return 0;
}

