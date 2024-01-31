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
#include "GameSerializer.hpp"
#include <imgui.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "EditorGUI.hpp"

int main(int argc, char* argv[])
{
	
	engine::EditorGUI editor{};
	editor.start();


	return 0;
}

