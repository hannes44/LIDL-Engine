#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "Window.hpp"

namespace engine
{
	class EditorGUI
	{
	public:
		EditorGUI();

		void renderNewFrame();

		void endFrame();
		
	private:
		void drawViewPort();

		void drawRightSidePanel();

		void drawLeftSidePanel();

		void drawTopMenu();

		void drawGuizmos();

		Window& window;
	};
}