#include "EditorGUI.hpp"
#include "Window.hpp"

namespace engine
{
#define IMGUI_TOP_MENU_HEIGHT 18
#define IMGUI_SHOW_DEMO_WINDOWS false

	EditorGUI::EditorGUI() : window(Window::getInstance())
	{
		
	}

	void EditorGUI::renderNewFrame()
	{
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		#if defined(_DEBUG) && IMGUI_SHOW_DEMO_WINDOWS 
			ImGui::ShowDemoWindow();
			ImGui::ShowStyleEditor();
		#endif 

		if (game == nullptr)
		{
			drawMainMenu();
		}
		else
		{
			drawViewPort();
			drawRightSidePanel();
			drawLeftSidePanel();
			drawTopMenu();
		}
	}

	void EditorGUI::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorGUI::drawMainMenu()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize(ImVec2(w, h));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("ViewPort", nullptr, windowFlags);
		ImGui::Text("Hello, world!");
		ImGui::End();
	}

	void EditorGUI::drawViewPort()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize(ImVec2(w, h));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoBackground;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("ViewPort", nullptr, windowFlags);

		bool isFocused = ImGui::IsWindowFocused();
		bool isHovered = ImGui::IsWindowHovered();
		if (!isFocused && !isHovered)
		{
			ImGui::GetIO().WantCaptureMouse = true;
			ImGui::GetIO().WantCaptureKeyboard = true;
		}
		else
		{
			ImGui::GetIO().WantCaptureMouse = false;
			ImGui::GetIO().WantCaptureKeyboard = false;
		}

		drawGuizmos();

		ImGui::End();
	}

	void EditorGUI::drawRightSidePanel()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		int panelWidth = w / 5;
		ImGui::SetNextWindowPos(ImVec2(w - panelWidth, IMGUI_TOP_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(panelWidth, h));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("##RightPanel", nullptr, windowFlags);
		if (ImGui::BeginTabBar("##RightPanelTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
		{
			ImGui::Text("Right Panel");

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void EditorGUI::drawLeftSidePanel()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		int panelWidth = w / 5;
		ImGui::SetNextWindowPos(ImVec2(0, IMGUI_TOP_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(panelWidth, h));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("##LeftPanel", nullptr, windowFlags);
		if (ImGui::BeginTabBar("##LeftPanelTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
		{
			ImGui::Text("Left Panel");

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void EditorGUI::drawTopMenu()
	{
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
			}
		}

		ImGui::EndMainMenuBar();
	}

	void EditorGUI::drawGuizmos()
	{
	}

}
