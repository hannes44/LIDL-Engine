#include "MainMenuGUI.hpp"
#include "imgui.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "Serializer/EditorSerializer.hpp"
#include "Serializer/GameSerializer.hpp"
#include "../Project.hpp"
#include "GUIHelper.hpp"
#include "Editor.hpp"
#include <imgui_internal.h>
#include <Windows.h>

namespace engine
{
	std::shared_ptr<engine::Project> MainMenuGUI::MainMenu()
	{
		// Initial project path
		strcpy(projectPath, EditorSerializer::getPathToEditorGamesFolder().c_str());

		while (true)
		{
			renderNewFrame();

			InputFramework::getInstance().getInput();

			renderMainMenu();


			endFrame();
			window.newFrame();
		}

		return std::shared_ptr<engine::Project>();
	}

	void MainMenuGUI::renderNewFrame()
	{
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void MainMenuGUI::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void MainMenuGUI::renderMainMenu()
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

		ImGui::Begin("MainMenu", nullptr, windowFlags);
		

		renderHeader();

		if (state == MainMenuState::NewProject)
		{
			renderNewProjectTab();
		}
		else if (state == MainMenuState::OpenProject)
		{
			renderOpenProjectTab();
		}

		renderFooter();

		ImGui::End();
	}

	void MainMenuGUI::renderHeader()
	{
		GUIHelper::drawCenteredText("GIGA Editor");

		// Very ugly solution to center the buttons
		// https://github.com/ocornut/imgui/discussions/3862
		ImGuiStyle& style = ImGui::GetStyle();

		float buttonWidth = 200.0f;
		float spacing = 50;

		float width = 0.0f;

		width += spacing;
		width += buttonWidth * 2;

		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - width) * 0.5;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		
		bool pushedColor = false;
		if (state == MainMenuState::NewProject)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.5f, 0.5f, 1.0f));
			pushedColor = true;
		}
		if (ImGui::Button("Create Project", { buttonWidth,50 })) {
			state = MainMenuState::NewProject;
		}
		if (state == MainMenuState::NewProject && pushedColor)
		{
			ImGui::PopStyleColor();
		}

		ImGui::SameLine(0, spacing);
		
		pushedColor = false;
		if (state == MainMenuState::OpenProject)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.5f, 0.5f, 1.0f));
			pushedColor = true;
		}
		if (ImGui::Button("Open Project", { buttonWidth,50 })) {
			state = MainMenuState::OpenProject;
		}
		if (state == MainMenuState::OpenProject && pushedColor)
		{
			ImGui::PopStyleColor();
		}

		ImGui::Separator();
	}

	void MainMenuGUI::renderFooter()
	{

	}

	void MainMenuGUI::renderNewProjectTab()
	{
		// flags
		ImGuiInputTextFlags flags = 0;

		ImGui::Text("Project Name: ");
		ImGui::SameLine();
		ImGui::InputText("##projectName", projectName, inputBufferSize, flags);

		ImGui::Text("Path:         ");
		ImGui::SameLine();
		ImGui::InputText("##projectPath", projectPath, inputBufferSize, flags);
		ImGui::SameLine();

		bool isProjectNameValid = EditorSerializer::isProjectNameValid(projectName);
		bool isProjectPathValid = EditorSerializer::isProjectPathValid(projectPath);

		if (ImGui::Button("Browse (Gunilla)"))
		{
			LOG_INFO("Getting path from windows file explorer");
			// TODO: Do this on another thread to avoid blocking the main thread
			std::string path = EditorSerializer::getFolderPathFromFileExplorer();
			LOG_INFO("Got path from windows file explorer: {}", path);
			if (!path.empty())
			{
				strcpy(projectPath, path.c_str());
			}
		}

		if (!isProjectNameValid || !isProjectPathValid)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Button("Create Project ##create project button"))
		{
			Editor::createNewProject(projectName, projectPath);
		}

		if (!isProjectNameValid || !isProjectPathValid)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		if (!isProjectNameValid)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Project name is invalid");
		}

		if (!isProjectPathValid)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Project path is invalid");
		}
	}

	void MainMenuGUI::renderOpenProjectTab()
	{
		ImGui::Text("Projects: ");
		for (auto gameName : EditorSerializer::getAllGameNamesInGamesFolder())
		{
			if (ImGui::Button(gameName.c_str()))
			{


			}
		}
	}
}
