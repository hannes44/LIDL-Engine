#include "EditorGUI.hpp"
#include <Engine.hpp>
#include "../../vendor/ImGuizmo/ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include "Serializer/EditorSerializer.hpp"
#include "Serializer/GameSerializer.hpp"
#include <Physics/GamePhysics.hpp>

namespace engine
{
#define IMGUI_TOP_MENU_HEIGHT 18
#define IMGUI_SHOW_DEMO_WINDOWS false

	EditorGUI::EditorGUI(std::shared_ptr<Project> project) :  window(Window::getInstance()), project(project)
	{
		game = project->game;
	}

	void EditorGUI::start()
	{
		game->initialize(); // Temporary for testing, should not be called when serialization works
		game->camera.translate(0, 0, 5);

		editorCamera.translate(0, 0, 15);
		editorCamera.rotate(10, 0, 1, 0);

		engine::Renderer::baseShader = engine::Shader::create("simple.vert", "simple.frag");

		assetManager = std::make_unique<AssetManager>(game.get());


		InputFramework& inputFramework = InputFramework::getInstance();
		inputFramework.addListener(this);

		assetManager->buildAssetTree();
   
		selectedAssetNodeFolder = assetManager->rootNode;

		editorSettings = EditorSerializer::deSerializeEditorSettings();

		rotateIconTexture = std::shared_ptr<Texture>(Texture::create("rotation_icon.png"));

		scaleIconTexture = std::shared_ptr<Texture>(Texture::create("scale_icon.png"));

		translateIconTexture = std::shared_ptr<Texture>(Texture::create("translation_icon.png"));

		worldIconTexture = std::shared_ptr<Texture>(Texture::create("world_icon.png"));

		while (true)
		{
			editorCamera.rotate(1, 0, 1, 0);
			renderNewFrame();

			inputFramework.getInput();

			Renderer::renderGame(game.get(), getActiveCamera(), &editorSettings.rendererSettings);

			if (sceneState == EditorSceneState::Play)
			{
				GamePhysics::getInstance().run(game.get());
				game->update();
			}

			// GamePhysics::getInstance().run(game.get());
			// game->run();
			// Renderer::renderGame(game, getActiveCamera(), &editorSettings.rendererSettings);

			endFrame();
			window.newFrame();
		}

		EditorSerializer::serializeEditorSettings(editorSettings);
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
			drawPlayButtonToolbar();
			drawBottomPanel();


		}
	}

	void EditorGUI::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorGUI::handleInput(const InputEvent& event, const std::string& EventType)
	{
		if (EventType == "KeyDown")
		{
			if ((Key)event.getKey() == Key::DELETE)
			{
				if (auto lockedSelectedObject = selectedObject.lock())
				{
					if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
					{
						game->deleteGameObject(lockedGameObject->getUUID().id);
					}
				}
			}
		}
	}

	void EditorGUI::drawMainMenu()
	{

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

		if (std::dynamic_pointer_cast<GameObject>(selectedObject.lock()))
			drawGuizmoOperationsWindow();

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
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("##RightPanel", nullptr, windowFlags);
		if (ImGui::BeginTabBar("##RightPanelTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
		{
			if (auto lockedSelectedObject = selectedObject.lock())
			{
				ImGui::Text(lockedSelectedObject->getUUID().id.c_str());

				if (dynamic_pointer_cast<GameObject>(lockedSelectedObject))
				{
					drawInspectorSelectedGameObject();
				}

			}
			else
			{
				drawGameSettingsTab();
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
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
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
				EditorSerializer::serializeEditorSettings(editorSettings);
				GameSerializer::serializeGame(game.get());
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

	// TODO: Style this, only for functionality currently
	void EditorGUI::drawPlayButtonToolbar()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		int panelWidth = w / 5;
		ImGui::SetNextWindowPos(ImVec2(panelWidth, IMGUI_TOP_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(w - panelWidth * 2, 30));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("##Playbutton", nullptr, windowFlags);

		if (ImGui::BeginTabBar("##BottomTabs", ImGuiTabBarFlags_None))
		{
			ImGuiTabBarFlags tabFlags = ImGuiSelectableFlags_None;
			
			if (wasStopButtonPressed)
				tabFlags = ImGuiTabItemFlags_SetSelected;
			

			if (ImGui::BeginTabItem("Scene", nullptr, tabFlags))
			{
				activeViewPort = ActiveViewPort::Scene;
				ImGui::EndTabItem();
			
			}

			tabFlags = ImGuiSelectableFlags_None;

			if (wasPlayButtonPressed)
				tabFlags = ImGuiTabItemFlags_SetSelected;

			if (ImGui::BeginTabItem("Game", nullptr, tabFlags))
			{
				activeViewPort = ActiveViewPort::Game;
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(150.0f, 20.0f));
		ImGui::SameLine();

		wasPlayButtonPressed = false;

		bool pushedStyleColor = false;
		if (sceneState == EditorSceneState::Play)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 100, 255));
			pushedStyleColor = true;
		}
		if (ImGui::Button("Play"))
		{
			wasPlayButtonPressed = true;

			sceneState = EditorSceneState::Play;

		}
		if (sceneState == EditorSceneState::Play && pushedStyleColor)
		{
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();

		wasStopButtonPressed = false;

		pushedStyleColor = false;
		if (sceneState == EditorSceneState::Scene)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 100, 255));
			pushedStyleColor = true;
		}
		if (ImGui::Button("Stop"))
		{
			sceneState = EditorSceneState::Scene;

			wasStopButtonPressed = true;
		}
		if (sceneState == EditorSceneState::Scene && pushedStyleColor)
		{
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}

	void EditorGUI::drawGuizmos()
	{
		// Only draw guizmos in scene view
		if (!editorSettings.showGizmos || activeViewPort != ActiveViewPort::Scene)
			return;

		bool shouldDrawGuizmos = false;

		float* modelMatrixPtr = nullptr;
		if (auto lockedSelectedObject = selectedObject.lock())
		{
			if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
			{
				shouldDrawGuizmos = true;
				modelMatrixPtr = glm::value_ptr(lockedGameObject->transform.transformMatrix);
			}
		}

		if (shouldDrawGuizmos)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			int windowWidth, windowHeight;
			window.getWindowSize(&windowWidth, &windowHeight);
			ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);

			glm::mat4 cameraView = getActiveCamera()->getViewMatrix();

			glm::mat4 projectionMatrix = getActiveCamera()->getProjectionMatrix();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(projectionMatrix), guizmoOperation, isGuizmoOperationInWorldSpace ? ImGuizmo::WORLD : ImGuizmo::LOCAL, modelMatrixPtr);
		}
	}

	void EditorGUI::drawInspectorSelectedGameObject()
	{
		if (auto lockedSelectedObject = selectedObject.lock())
		{
			if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
			{ 
				ImGui::Text("Name: ");
				//	ImGui::SameLine();
				//	strcpy(selectedItemNameBuffer, selectedItem.lock()->getName().c_str());
				//	ImGui::InputText("##selectedItemNameInput", selectedItemNameBuffer, 255);
				//	selectedItemNameBuffer, selectedItem.lock()->getName() = selectedItemNameBuffer;

				// Since all gameobjects have a transform, we can always draw the transform
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					float matrixTranslation[3], matrixRotation[3], matrixScale[3];
					ImGuizmo::DecomposeMatrixToComponents(&(lockedGameObject->transform.transformMatrix[0][0]), matrixTranslation, matrixRotation, matrixScale);

					ImGui::InputFloat3("Translation", matrixTranslation);
					ImGui::InputFloat3("Rotation", matrixRotation);
					ImGui::InputFloat3("Scale", matrixScale);

					ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &(lockedGameObject->transform.transformMatrix[0][0]));
				}

			
				for (auto component : lockedGameObject->components)
				{
					if (ImGui::CollapsingHeader(component->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
					}
				}
			
			
			}
		}	
	}
	void EditorGUI::drawGameSettingsTab()
	{
		ImGui::Text("Application FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Editor Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("RENDERING SETTINGS");
			defaultCheckBox("Multisampling", &editorSettings.rendererSettings.useMultiSampling);
			defaultCheckBox("Show Triangle outlines", &editorSettings.rendererSettings.drawWireframe);
			defaultCheckBox("Depth Test", &editorSettings.rendererSettings.enableDepthTest);
			defaultCheckBox("Face Culling", &editorSettings.rendererSettings.enableFaceCulling);

			ImGui::Text("EDITOR SETTINGS");
			defaultCheckBox("Show Gizmos", &editorSettings.showGizmos);

			bool savedUseDarkTheme = editorSettings.useDarkTheme;
			if (defaultCheckBox("Use Dark Mode", &editorSettings.useDarkTheme))
			{
				if (savedUseDarkTheme != editorSettings.useDarkTheme)
					ImGui::StyleColorsDark();
			}
			else if (!editorSettings.useDarkTheme)
			{
				ImGui::StyleColorsLight();
			}

			ImGui::Text("Camera Settings");
			ImGui::SliderFloat("Camera Speed", &editorCamera.movementSpeed, 0.001f, 1.0f);
			ImGui::SliderFloat("Camera Sensitivity", &editorCamera.rotationSpeed, 0.001f, 0.1f); 
			ImGui::SliderFloat("Camera FOV", &editorCamera.fov, 0.1f, 120.0f);
			
		}
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Game Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Game Name: ");
		}
		ImGui::Separator();

	}

	void EditorGUI::drawBottomPanel()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		int panelWidth = w / 5;

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;


		ImGui::SetNextWindowPos(ImVec2(panelWidth, h - 300));
		ImGui::SetNextWindowSize(ImVec2(w - panelWidth * 2, 300));
		ImGui::Begin("##BottomPanel", nullptr, windowFlags);

		if (ImGui::BeginTabBar("##BottomTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Assets"))
			{
				drawAssetsSection();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Console"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void EditorGUI::drawAssetsSection()
	{
		if (auto lockedSelectedAssetNodeFolder = selectedAssetNodeFolder.lock())
		{
			std::vector<std::weak_ptr<AssetNode>> parents = lockedSelectedAssetNodeFolder->getEntireParentage();
			std::reverse(parents.begin(), parents.end());

			for (auto parent : parents)
			{
				if (auto lockedParent = parent.lock())
				{
					ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
					if (ImGui::Button(lockedParent->name.c_str()))
					{
						selectedAssetNodeFolder = lockedParent;
						ImGui::PopStyleColor();
						break;
					}

					ImGui::PopStyleColor();


					ImGui::SameLine();
					ImGui::Text(">");
					ImGui::SameLine();
				}
			}


			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
			ImGui::Button(lockedSelectedAssetNodeFolder->name.c_str());
			ImGui::PopStyleColor();

			ImGui::Separator();



			// draw the assets in the selected folder
			for (auto& child : lockedSelectedAssetNodeFolder->children)
			{
				if (child == NULL)
					continue;

				drawAssetItem(child);
				ImGui::SameLine();
			}
		}
	}

	void EditorGUI::drawGuizmoOperationsWindow()
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::SetNextWindowSize(ImVec2(50, 170));
		ImGui::Begin("Gizmo Operation", nullptr, windowFlags);

		bool pushedStyleColor = false;
		if (guizmoOperation == ImGuizmo::ROTATE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##rotateOperationButton", (void*)(intptr_t)rotateIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
		{
			guizmoOperation = ImGuizmo::ROTATE;
		}
		if (pushedStyleColor)
			ImGui::PopStyleColor();

		pushedStyleColor = false;


		if (guizmoOperation == ImGuizmo::TRANSLATE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##translateOperationButton", (void*)(intptr_t)translateIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
		{
			guizmoOperation = ImGuizmo::TRANSLATE;
		}
		if (pushedStyleColor)
			ImGui::PopStyleColor();


		pushedStyleColor = false;

		if (guizmoOperation == ImGuizmo::SCALE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##scaleOperationButton", (void*)(intptr_t)scaleIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
		{
			guizmoOperation = ImGuizmo::SCALE;
		}
		if (pushedStyleColor)
			ImGui::PopStyleColor();


		pushedStyleColor = false;

		ImGui::NewLine();

		if (isGuizmoOperationInWorldSpace)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##worldOperationButton", (void*)(intptr_t)worldIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
		{
			isGuizmoOperationInWorldSpace = !isGuizmoOperationInWorldSpace;
		}
		if (pushedStyleColor)
			ImGui::PopStyleColor();

		ImGui::End();
	}

	void EditorGUI::drawAssetItem(std::shared_ptr<AssetNode> assetNode)
	{
		ImGui::BeginGroup();
		{
			int openGLTextureId = assetNode->iconTexture != nullptr ? assetNode->iconTexture->textureIDOpenGL : 0;

			if (ImGui::ImageButton(("##" + assetNode->uuid.id).c_str(), (void*)(intptr_t)openGLTextureId, ImVec2(70, 70), { 0, 1 }, { 1, 0 }))
			{
				if (assetNode->isFolder)
					selectedAssetNodeFolder = assetNode;
			}


			float currentX = ImGui::GetCursorPosX();
			std::string name = assetNode->name;

			if (name.size() > 10)
				name = name.substr(0, 8) + "...";

			auto textWidth = ImGui::CalcTextSize(name.c_str()).x;

			ImGui::SetCursorPosX(currentX + (75 - textWidth) / 2);

			ImGui::Text(name.c_str());
		}
		ImGui::EndGroup();
	}

	bool EditorGUI::defaultCheckBox(const std::string& label, bool* value)
	{
		return ImGui::Checkbox(label.c_str(), value);
	}

	Camera* EditorGUI::getActiveCamera()
	{
		if (activeViewPort == ActiveViewPort::Scene)
			return &editorCamera;
		else 
			return &game->camera;

	}
	void EditorGUI::changeGame(std::shared_ptr<Game> game)
	{
		this->game = game;

		assetManager->changeGame(game.get());
	}
}
