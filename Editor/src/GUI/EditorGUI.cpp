#include "EditorGUI.hpp"
#include <Engine.hpp>
#include "Input/InputEvent.hpp"
#include "../../vendor/ImGuizmo/ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include "Audio/AudioManager.hpp"
#include "Serializer/EditorSerializer.hpp"
#include "Serializer/GameSerializer.hpp"
#include <Physics/GamePhysics.hpp>
#include "Components/ComponentFactory.hpp"
#include <memory>
#include <imgui_internal.h>
#include <Windows.h>
#include <regex>
#include <ShlDisp.h>


namespace engine
{
#define IMGUI_TOP_MENU_HEIGHT 18
#define IMGUI_SHOW_DEMO_WINDOWS false

// We have to undefine DELETE because it is causing a conflict with the InputEvent DELETE
#undef DELETE

bool isAddComponentVisible = false;

	EditorGUI::EditorGUI(std::shared_ptr<Project> project, EditorSettings& editorSettings) :  window(Window::getInstance()), project(project), editorSettings(editorSettings)
	{
		game = project->game;
	}

	void EditorGUI::start()
	{
		ActionMap::getInstance().addAction("Copy", {Key::LCTRL, Key::C});

		if (editorSettings.enableScripting)
		{
			ScriptEngine* scriptEngine = ScriptEngine::getInstance();
			scriptEngine->loadScriptStatesIntoNewLuaState(project->game.get());
		}

		// We have to save the initial serialization state to avoid serializing the initiated game if the user changes settings
		bool initialUseSerialization = editorSettings.useSerialization;
		if (initialUseSerialization)
		{
			GameSerializer::deserializeGame(game.get());
		}
		else
		{
			game->initialize();
		}
		
		AudioManager::getInstance().initialize();

		Renderer* renderer = Renderer::getInstance();

		assetManager = std::make_unique<AssetManager>(game.get());

		InputFramework& inputFramework = InputFramework::getInstance();
		inputFramework.addListener(this);

		inputFramework.addListener(&editorCamera);

		EventManager& eventManager = EventManager::getInstance();
		eventManager.subscribe(EventType::QuitProgram, this);

		assetManager->buildAssetTree();
   
		selectedAssetNodeFolder = assetManager->rootNode;

		rotateIconTexture = std::shared_ptr<Texture>(Texture::create("rotation_icon.png", false));

		scaleIconTexture = std::shared_ptr<Texture>(Texture::create("scale_icon.png", false));

		translateIconTexture = std::shared_ptr<Texture>(Texture::create("translation_icon.png", false));

		worldIconTexture = std::shared_ptr<Texture>(Texture::create("world_icon.png", false));


		
		while (!quitProgram)
		{
			renderNewFrame();

			inputFramework.getInput();

			renderer->renderGame(game.get(), getActiveCamera(), &editorSettings.rendererSettings);
			renderer->renderGizmos(game.get(), getActiveCamera(), &editorSettings.rendererSettings);

			// Checking if any scripts have been updated
			// TODO: This doesn't have to be done every frame
			ScriptEngine::getInstance()->checkForUpdatedScripts();

			if (sceneState == EditorSceneState::Play)
			{
				GamePhysics::getInstance().run(game.get());
				game->update();

				for (auto& [gameObjectId, gameObject] : game->getGameObjects())
				{
					gameObject->update();
				}
			}

			endFrame();
			window.newFrame();
		}

		if (initialUseSerialization)
			GameSerializer::serializeGame(game.get());
		
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

			if (!ScriptEngine::getInstance()->isSuccessfullyCompiled)
			{
				drawCompilationErrorWindow();
			}
		}
	}

	void EditorGUI::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorGUI::handleInput(const InputEvent& event)
	{
		InputEventType EventType = event.getEventType();
		if (EventType == InputEventType::KeyDown)
		{
			if ((Key)event.getKey() == Key::DELETE)
			{
				AudioManager::getInstance().playSound("boing_x.wav");
				if (auto lockedSelectedObject = selectedObject.lock())
				{
					if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
					{
						std::string gameObjectId = lockedGameObject->getUUID().id;
						game->deleteGameObject(lockedGameObject->getUUID().id);
						EventManager::getInstance().notify(EventType::SelectableDeleted, gameObjectId);
					}
					else if (auto lockedMaterial = dynamic_pointer_cast<Material>(lockedSelectedObject))
					{
						std::string materialId = lockedMaterial->getUUID().id;
						game->deleteMaterial(lockedMaterial->getUUID().id);
						EventManager::getInstance().notify(EventType::SelectableDeleted, materialId);
					}
					else if (auto lockedTexture = dynamic_pointer_cast<Texture>(lockedSelectedObject))
					{
						std::string textureId = lockedTexture->getUUID().id;
						game->deleteTexture(lockedTexture->getUUID().id);
						EventManager::getInstance().notify(EventType::SelectableDeleted, textureId);
					}
				}
			}

			if ((Key)event.getKey() == Key::V)
			{
				if (auto lockedCopiedGameObject = copiedGameObject.lock())
				{
					LOG_INFO("Pasting game object");
					GameObject newGameObject = lockedCopiedGameObject->clone();
					std::shared_ptr<GameObject> newGameObjectPtr = std::make_shared<GameObject>(newGameObject);
					game->addGameObject(newGameObjectPtr);
					selectedObject = newGameObjectPtr;
				}
			}
		}

		if (event.getAction() == "Copy")
		{
			if (auto lockedSelectedObject = selectedObject.lock())
			{
				if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
				{
					LOG_INFO("Copying game object");
					copiedGameObject = lockedGameObject;
				}
			}
		}
	}

	void EditorGUI::onEvent(EventType type, std::string message)
	{
		if (type == EventType::QuitProgram)
		{
			quitProgram = true;
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
				else if (dynamic_pointer_cast<Serializable>(lockedSelectedObject))
				{
					drawSerializableVariables(dynamic_pointer_cast<Serializable>(lockedSelectedObject).get());
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
				for (const auto& [gameObjectId, gameObject] : game->getGameObjects())
				{
					ImGui::PushID(gameObjectId.c_str());
					if (ImGui::Selectable(gameObject->name.c_str(), selectedObject.lock() && (gameObject->getUUID() == selectedObject.lock()->getUUID())))
					{
						selectedObject = gameObject;
					}

					if (ImGui::BeginPopupContextItem())
					{
						static char name[32];
						memcpy(name, gameObject->name.c_str(), 32);
						char buf[64];
						sprintf(buf, "%s###Button", name);
						ImGui::Button(buf);
						if (ImGui::BeginPopupContextItem("Test"))
						{
							ImGui::Text("Edit name:");
							ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							gameObject->name = name;
							ImGui::EndPopup();
						}
						ImGui::Separator();
						if (ImGui::Button("Close"))
							ImGui::CloseCurrentPopup();
						ImGui::EndPopup();
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
		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::MenuItem("Create Material"))
			{
				LOG_INFO("Create Material Pressed");
				std::weak_ptr<Material> material = game->createMaterial("Material");
				EventManager::getInstance().notify(EventType::SelectableAdded, material.lock()->getUUID().id);
				selectedObject = material;
			}
			if (ImGui::MenuItem("Add texture file"))
			{
				char filter[256] = "png or jpeg: \0*.PNG;*.png;*.jpeg;*.jpg\0\0";
				std::string textureFileName = ResourceManager::addFileFromWindowsExplorerToProject(filter);
				if (textureFileName != "")
				{
					std::weak_ptr<Texture> texture = game->createTexture(textureFileName);
					EventManager::getInstance().notify(EventType::SelectableAdded, texture.lock()->getUUID().id);
					selectedObject = texture;
				}
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
					std::shared_ptr<GameObject> cube = std::make_shared<GameObject>();
					cube->name = "Cube";
					cube->addComponent(MeshComponent::createPrimative(PrimativeMeshType::CUBE));
					game->addGameObject(cube);
					selectedObject = cube;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					std::shared_ptr<GameObject> sphere = std::make_shared<GameObject>();
					sphere->name = "Sphere";
					sphere->addComponent(MeshComponent::createPrimative(PrimativeMeshType::SPHERE));
					game->addGameObject(sphere);
					selectedObject = sphere;
				}
				if (ImGui::MenuItem("Plane"))
				{
					std::shared_ptr<GameObject> plane = std::make_shared<GameObject>();
					plane->name = "Plane";
					plane->addComponent(MeshComponent::createPrimative(PrimativeMeshType::PLANE));
					game->addGameObject(plane);
					selectedObject = plane;
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Add obj file mesh"))
			{
				char fileFilter[64] = "obj files: .obj\0*.obj*\0\0";
				std::string filename = ResourceManager::addFileFromWindowsExplorerToProject(fileFilter);
				if (filename != "")
				{		
					std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
					obj->name = filename;
					obj->addComponent(MeshComponent::createMeshFromObjFile(filename));
					game->addGameObject(obj);
					selectedObject = obj;
					
				}
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
				ShellExecute(NULL, "open", "https://github.com/hannes44/GameEngineTDA572", 0, 0, SW_SHOWDEFAULT);
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

			// Only start the game if it isn't already playing
			if (sceneState != EditorSceneState::Play)
			{
				playGame();
			}
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
			wasStopButtonPressed = true;

			if (sceneState != EditorSceneState::Scene)
			{
				stopGame();
			}

			
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
				ImGui::SameLine();
				static char gameObjectNameBuffer[255];
				strcpy(gameObjectNameBuffer, lockedGameObject->name.c_str());
				ImGui::InputText("##GameObjectNameInput", gameObjectNameBuffer, 255);
				lockedGameObject->name = gameObjectNameBuffer;

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

			
				for (auto component : lockedGameObject->getComponents())
				{
					std::string componentName = component->getName();

					// Special case for scriptable components
					if (auto lockedScriptable = dynamic_pointer_cast<ScriptableComponent>(component))
					{
						componentName = lockedScriptable->getScriptClassName();
					}

					if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						drawSerializableVariables(component.get());
					}
				}

				if (ImGui::Button("Add Component"))
				{
					isAddComponentVisible = !isAddComponentVisible;
				}
			
			}
		}
		if (isAddComponentVisible)
		{
			ShowAddComponent();
		}
	}
	void EditorGUI::ShowAddComponent()
	{
		ImGuiTextFilter     Filter;

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		ImGui::SetNextWindowSize(ImVec2(360, 500));

		if (ImGui::Begin("Add Component", nullptr, windowFlags))
		{
			ImGui::Text("Add Component");
			ImGui::Separator();

			std::vector<std::string> allComponentNames = { "Box Collider", "Camera", "Mesh", "Physics", "PointLight", "Sphere Collider" };
			std::vector<std::string> scriptComponentNames = ResourceManager::getInstance()->getAllCSharpScriptsInActiveGame();
		
			// Remove the extension from the script names
			for (auto& scriptName : scriptComponentNames)
			{
				scriptName = scriptName.substr(0, scriptName.find_last_of('.'));
			}

			allComponentNames.insert(allComponentNames.end(), scriptComponentNames.begin(), scriptComponentNames.end());

			if (ImGui::BeginListBox("##"))
			{
				for (auto componentName : allComponentNames)
				{
					if (ImGui::Selectable(componentName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							isAddComponentVisible = !isAddComponentVisible;

							if (auto lockedSelectedObject = selectedObject.lock())
							{
								if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
								{
									lockedGameObject->addComponent(ComponentFactory::createComponent(componentName));
								}
							}

						}
					}
				}
				ImGui::EndListBox();
			}
			if (ImGui::Button("Close"))
			{
				isAddComponentVisible = !isAddComponentVisible;
			}
		}
		ImGui::End();
	}
	void EditorGUI::drawGameSettingsTab()
	{
		ImGui::Text("Application FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Editor Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("RENDERING SETTINGS");

			drawSerializableVariables(&editorSettings.rendererSettings);

			ImGui::Text("EDITOR SETTINGS");

			drawSerializableVariables(&editorSettings);

			if (editorSettings.useDarkTheme)
			{
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
			ImGui::Text(("Game Name: " + game->name).c_str());
			
			ImGui::Text("Physics Settings");
			drawSerializableVariables(&game->config.physicsSettings);
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
				if(ImGui::SmallButton("New Folder"))
				{
					if (auto lockedSelectedAssetNodeFolder = selectedAssetNodeFolder.lock())
					{
						std::shared_ptr<AssetNode> newFolder = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
						newFolder->name = "New Folder";
						
						int i = 1;
						std::string newFolderName = newFolder->name;
						while (assetManager->isNameInUse(lockedSelectedAssetNodeFolder, newFolderName))
						{
							newFolderName = newFolder->name + " (" + std::to_string(i) + ")";
							i++;
						}
						newFolder->name = newFolderName;

						assetManager->addChild(lockedSelectedAssetNodeFolder, newFolder);
					}

				}


				static char newScriptName[64];

				ImGui::SameLine();

				if (ImGui::SmallButton("New Script"))
				{
					std::string scriptName = newScriptName + std::string(".cs");
					ResourceManager::getInstance()->createNewScriptForActiveGame(scriptName);
					assetManager->addNewScriptNode(scriptName);
				}

				ImGui::SameLine();

				ImGui::InputText("##newScriptName", newScriptName, 64);

				drawAssetsSection();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Console"))
			{
				ConsoleGUI::ShowConsole();
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

	void EditorGUI::drawSerializableVariables(Serializable* serializable)
	{
		for (auto seralizableVariable : serializable->getSerializableVariables())
		{
			if (!seralizableVariable.showInEditor)
				continue;

			if (seralizableVariable.data == nullptr)
			{
				ImGui::Text((seralizableVariable.name + ":").c_str());

				continue;
			}

			if (seralizableVariable.type == SerializableType::STRING)
			{
				std::string data = *static_cast<std::string*>(seralizableVariable.data);
				ImGui::Text((seralizableVariable.name + ":").c_str());
				ImGui::SameLine();
				char name[64];
				memcpy(name, data.c_str(), 64);
				ImGui::InputText(("##"+seralizableVariable.name).c_str(), name, IM_ARRAYSIZE(name));
				*static_cast<std::string*>(seralizableVariable.data) = name;

				//ImGui::Text(data.c_str());
			}
			else if (seralizableVariable.type == SerializableType::INT)
			{
				ImGui::InputInt(seralizableVariable.name.c_str(), (int*)seralizableVariable.data);
			}
			else if (seralizableVariable.type == SerializableType::FLOAT)
			{
				ImGui::InputFloat(seralizableVariable.name.c_str(), (float*)seralizableVariable.data); 
			}
			else if (seralizableVariable.type == SerializableType::BOOLEAN)
			{
				ImGui::Checkbox(seralizableVariable.name.c_str(), (bool*)seralizableVariable.data);
			}
			else if (seralizableVariable.type == SerializableType::DOUBLE)
			{
				ImGui::InputDouble(seralizableVariable.name.c_str(), (double*)seralizableVariable.data);
			}
			else if (seralizableVariable.type == SerializableType::VECTOR2)
			{
				ImGui::InputFloat2(seralizableVariable.name.c_str(), (float*)seralizableVariable.data);
			}
			else if (seralizableVariable.type == SerializableType::VECTOR3)
			{
				ImGui::InputFloat3(seralizableVariable.name.c_str(), (float*)seralizableVariable.data);
			}
			else if (seralizableVariable.type == SerializableType::COLOR)
			{
				ImGui::ColorEdit3(seralizableVariable.name.c_str(), (float*)seralizableVariable.data);
			}
			else if (seralizableVariable.type == SerializableType::VECTOR4)
			{
				ImGui::InputFloat4(seralizableVariable.name.c_str(), (float*)seralizableVariable.data);
			}
			else {
				LOG_WARN("Cannot serialize variable type: name: {}", seralizableVariable.name);
				return;
			}

			ImGui::SameLine();
			GUIHelper::HelpMarker(seralizableVariable.description.c_str());
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
			int openGLTextureId = !assetNode->getIconTexture().expired() ? assetNode->getIconTexture().lock()->textureIDOpenGL : 0;

			if (ImGui::ImageButton(("##" + assetNode->uuid.id).c_str(), (void*)(intptr_t)openGLTextureId, ImVec2(70, 70), { 0, 1 }, { 1, 0 }))
			{
				// Special case for script
				if (assetNode->isScript)
				{
					// Opening the script in the default editor for the user
					std::string path = ResourceManager::getInstance()->getPathToGameResource(assetNode->name);
					std::string const pathCorrectFormat = std::regex_replace(path, std::regex("/"), "\\");
					LPCSTR filePath = pathCorrectFormat.c_str();
					ShellExecute(0, 0, filePath, NULL, NULL, SW_SHOW);
				}
				else if (assetNode->isFolder)
					selectedAssetNodeFolder = assetNode;
				else
					selectedObject = assetNode->asset;
			}

			if (ImGui::BeginPopupContextItem())
			{
				static char name[32];
				memcpy(name, assetNode->name.c_str(), 32);
            	char buf[64];
            	sprintf(buf, "%s###Button", name);
				ImGui::Button(buf);
				if (ImGui::BeginPopupContextItem())
				{
					ImGui::Text("Edit name:");
					ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					assetNode->name = name;
					ImGui::EndPopup();
				}
				ImGui::Separator();
				if (ImGui::Button("Delete"))
				{
					assetManager->deleteAssetNode(assetNode);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
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

	void EditorGUI::drawCompilationErrorWindow()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowSize(ImVec2(800, 500));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Compilation Error", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Compilation Error");

		// Set color to red
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::TextWrapped(ScriptEngine::getInstance()->lastCompilationError.c_str());
		ImGui::PopStyleColor();
		ImGui::Text("");
		ImGui::Text("Fix the compilation error to use the Editor!");
		ImGui::End();
	}

	bool EditorGUI::defaultCheckBox(const std::string& label, bool* value)
	{
		return ImGui::Checkbox(label.c_str(), value);
	}

	void EditorGUI::playGame()
	{
		// Save the current state of the game
		GameSerializer::serializeGame(game.get());

		for (auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			gameObject->initialize();
		}
		sceneState = EditorSceneState::Play;
	}

	void EditorGUI::stopGame()
	{
		// Overwrite the current state of the game with the saved state
		GameSerializer::deserializeGame(game.get());

		// We should probably reset the sript states aswell since only serializable script variables will be reset

		sceneState = EditorSceneState::Scene;
	}

	CameraComponent* EditorGUI::getActiveCamera()
	{
		if (activeViewPort == ActiveViewPort::Scene)
		{
			return &editorCamera;
		}
		else
		{
			return game->getMainCamera();
		}
	}
	void EditorGUI::changeGame(std::shared_ptr<Game> game)
	{
		this->game = game;

		assetManager->changeGame(game.get());
	}
}
