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
#include <imgui.h>
#include <stdio.h>
#include <SDL3/SDL.h>


int main(int argc, char* argv[])
{
	engine::Logger::init();

	engine::Window::getInstance().createWindow(800, 600, "Sweddes MAMMA P� PIZZA");

	engine::Renderer::initGraphicsAPI(engine::GraphicsAPIType::OpenGL);
	
	glewInit();

	auto game = engine::TestGame();
	engine::GameSerializer::SerializeGame(game);

	engine::Renderer::baseShader = engine::Shader::create("simple.vert", "simple.frag");


	game.initialize();
	game.gameLoop();
			

	/*
	auto game = engine::TestGame();
	game.initialize();
	game.gameLoop();
	*/

	//ImGui::Text("Hello, world");

	return 0;
}

