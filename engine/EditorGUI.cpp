#include "EditorGUI.hpp"
#include "Window.hpp"
#include "Game.hpp"
#include "../vendor/ImGuizmo/ImGuizmo.h"


namespace engine
{
#define IMGUI_TOP_MENU_HEIGHT 18
#define IMGUI_SHOW_DEMO_WINDOWS true

	EditorGUI::EditorGUI() : window(Window::getInstance())
	{
		
	}

	void EditorGUI::renderNewFrame()
	{
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

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
			if (auto lockedSelectedObject = selectedObject.lock())
			{
				ImGui::Text(lockedSelectedObject->getUUID().id.c_str());

				if (dynamic_cast<GameObject*>(lockedSelectedObject.get()))
				{
					drawInspectorSelectedGameObject();
				}

			}

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
			if (ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
				ImGui::BeginListBox("##2", ImVec2(500, 500));
				for (const auto& [gameObjectId, gameObject] : game->gameObjects)
				{
					ImGui::PushID(gameObjectId.c_str());
					if (ImGui::Selectable(gameObject->name.c_str(), selectedObject.lock() && (gameObject->getUUID() == selectedObject.lock()->getUUID())))
					{
						selectedObject = gameObject;
					}
					ImGui::PopID();


				}

				ImGui::EndListBox();
				ImGui::PopStyleColor();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void EditorGUI::drawTopMenu()
	{
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Project"))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Open"))
			{
			}
			if (ImGui::BeginMenu("Open Recent"))
			{
				if (ImGui::MenuItem("Temporary"))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
			}
			if (ImGui::MenuItem("Save As"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				ImGui::End();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z"))
			{
			}
			if (ImGui::MenuItem("Redo", "Ctrl+Y"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X"))
			{
			}
			if (ImGui::MenuItem("Copy", "Ctrl+C"))
			{
			}
			if (ImGui::MenuItem("Paste", "Ctrl+P"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				game->addGameObject(std::make_unique<GameObject>());
			}
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Cube"))
				{
				}
				if (ImGui::MenuItem("Sphere"))
				{
				}
				if (ImGui::MenuItem("Plane"))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Hierarchy"))
			{
			}
			if (ImGui::MenuItem("Inspector"))
			{
			}
			if (ImGui::MenuItem("Game"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	void EditorGUI::drawGuizmos()
	{
	}

	void EditorGUI::drawInspectorSelectedGameObject()
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(selectedObject.lock().get());


		ImGui::Text("Name: ");

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(&(gameObject->transform.transformMatrix[0][0]), matrixTranslation, matrixRotation, matrixScale);

		ImGui::InputFloat3("Translation", matrixTranslation);
		ImGui::InputFloat3("Rotation", matrixRotation);
		ImGui::InputFloat3("Scale", matrixScale);

		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &(gameObject->transform.transformMatrix[0][0]));
	//	ImGui::SameLine();
	//	strcpy(selectedItemNameBuffer, selectedItem.lock()->getName().c_str());
	//	ImGui::InputText("##selectedItemNameInput", selectedItemNameBuffer, 255);
	//	selectedItemNameBuffer, selectedItem.lock()->getName() = selectedItemNameBuffer;
	}

}
