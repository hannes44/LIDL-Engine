#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "Window.hpp"
#include "Game.hpp"
#include "../vendor/ImGuizmo/ImGuizmo.h"

namespace engine
{
	class EditorGUI
	{
	public:
		EditorGUI();

		void start();

		void renderNewFrame();

		void endFrame();
		
		Game* game = nullptr;
	private:
		void drawMainMenu();

		void drawViewPort();

		void drawRightSidePanel();

		void drawLeftSidePanel();

		void drawTopMenu();

		void drawPlayButtonToolbar();

		void drawGuizmos();

		void drawInspectorSelectedGameObject();

		Window& window;

		std::weak_ptr<Selectable> selectedObject;

		ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;
	};
}