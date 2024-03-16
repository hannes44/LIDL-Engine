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
#include <set>
#include <ranges>
#include <imgui_internal.h>

#define _WINSOCKAPI_
#define NOMINMAX
#include <Windows.h>

#include <regex>
#include <ShlDisp.h>
#include "Utils/Utils.hpp"

#include <thread>
#include <fstream>
#include "MultiplayerClient/Client.hpp"
#include "Serializer/GameSerializer.hpp"
#include <filesystem>

namespace engine
{
#define IMGUI_TOP_MENU_HEIGHT 18
#define IMGUI_SHOW_DEMO_WINDOWS false

	// We have to undefine DELETE because it is causing a conflict with the InputEvent DELETE
#undef DELETE

	bool isAddComponentVisible = false;

	EditorGUI::EditorGUI(std::shared_ptr<Project> project, EditorSettings& editorSettings) : window(Window::getInstance()), project(project), editorSettings(editorSettings)
	{
		game = project->game;
	}

	void EditorGUI::onMultiplayerStateReceived(std::shared_ptr<Game> game, std::string state)
	{
		if (!game->running)
			return;

		std::string folderPath = MULTIPLAYER_STATE_FOLDER + game->instanceId + "/";
		std::filesystem::create_directory(folderPath);

		std::string filePath = folderPath + "ParsedState" + MULTIPLAYER_STATE_FILE_EXTENSION;

		multiplayerReceiveLock.lock();
		std::ofstream outfile(filePath);
		outfile << state.c_str();
		outfile.close();

		if (!std::ifstream(filePath).good()) {
			LOG_ERROR("File not found: " + filePath);
			return;
		}

		GameSerializer::updateGameState(game.get(), filePath);
		multiplayerReceiveLock.unlock();
	}

	void EditorGUI::setupMultiplayer(std::shared_ptr<Game> game) {
		if (!game->isMultiplayerGame())
			return;

		// Connect to the server
		multiplayerSocket = Client::OpenSocket();

		// Start the multiplayer receiver in a detached thread
		std::thread receiver(Client::RunReceiver, multiplayerSocket, std::bind(&EditorGUI::onMultiplayerStateReceived, this, game, std::placeholders::_1));

		// Start the multiplayer transmitter in a detached thread
		std::thread transmitter(Client::RunTransmitter, multiplayerSocket);

		receiver.detach();
		transmitter.detach();
	}

	void EditorGUI::start()
	{
		createEditorInputActions();

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

		EventManager& eventManager = EventManager::getInstance();
		eventManager.subscribe(EventType::QuitProgram, this);

		assetManager->buildAssetTree();

		selectedAssetNodeFolder = assetManager->rootNode;

		createEditorTextures();

		setupMultiplayer(game);

		setupEditorCamera();

		float deltaTime = 0.0f;

		while (!quitProgram)
		{
			deltaTime = ImGui::GetIO().DeltaTime;

			renderNewFrame();

			inputFramework.getInput();

			GamePhysics::getInstance().fixedUpdate(editorGameObjects, editorPhysicsSettings);

			if (noGUIMode)
			{
				// When in no GUI mode, render to entire window
				window.getWindowSize(&editorSettings.rendererSettings.width, &editorSettings.rendererSettings.height);
				Renderer::getInstance()->renderGame(game.get(), getActiveCamera(), &editorSettings.rendererSettings, glm::vec2(0));
			}
			else
			{
				Renderer::getInstance()->renderGame(game.get(), getActiveCamera(), &editorSettings.rendererSettings, viewPortPosition);
			}

			renderer->renderGizmos(game.get(), getActiveCamera(), &editorSettings.rendererSettings);

			// Checking if any scripts have been updated
			// TODO: This doesn't have to be done every frame
			ScriptEngine::getInstance()->checkForUpdatedScripts();

			if (sceneState == EditorSceneState::Play)
			{
				GamePhysics::getInstance().run(game.get());
				game->update();

				for (auto& [gameObjectId, gameObject] : game->getGameObjects())
					gameObject->update(deltaTime);
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
		if (window.isWindowMinimized())
			return;

		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

#if defined(_DEBUG) && IMGUI_SHOW_DEMO_WINDOWS
		ImGui::ShowDemoWindow();
		ImGui::ShowStyleEditor();
#endif

		if (noGUIMode)
			return;

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
				drawCompilationErrorWindow();
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
						game->deleteGameObjectFromId(lockedGameObject->getUUID().id);
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
			if ((Key)event.getKey() == Key::ESCAPE)
			{
				noGUIMode = !noGUIMode;
				window.setRelativeMouseMode(noGUIMode);
			}
		}
		if (EventType == InputEventType::MouseButtonDown)
		{
			// Select game object on mouse click
			if ((Key)event.getButton() == Key::MOUSE_LEFT)
			{
				// Only select objects if the mouse is inside the viewport and not over guizmos
				if (isMouseInsideViewPort() && !isMouseOverGuizmo && !isMouseOverGuizmosOperationWindow)
				{
					glm::vec3 rayDirection = Utils::getMouseRayDirection(window, *getActiveCamera(), viewPortSize, viewPortPosition);
					glm::vec3 rayOrigin = getActiveCamera()->getTransform().getPosition();

					auto gameObjects = Utils::getAABBGameObjectCollisions(game.get(), rayOrigin, rayDirection);
					if (gameObjects.size() > 0)
					{
						selectedObject = gameObjects[0];
					}
					else
					{
						selectedObject.reset();
					}
				}
			}
		}

		if (event.getAction() == "Paste" && event.getEventType() == InputEventType::ActionDown)
		{
			pasteGameObject();
		}

		if (event.getAction() == "Copy" && event.getEventType() == InputEventType::ActionDown)
		{
			copySelectedGameObject();	
		}

		if (event.getAction() == "Save" && event.getEventType() == InputEventType::ActionDown)
		{
			EditorSerializer::serializeEditorSettings(editorSettings);
			GameSerializer::serializeGame(game.get());
		}
	}

	void EditorGUI::onEvent(EventType type, std::string message)
	{
		if (type == EventType::QuitProgram)
			quitProgram = true;
	}

	void EditorGUI::drawMainMenu()
	{
	}

	void EditorGUI::drawViewPort()
	{
		int w, h;
		window.getWindowSize(&w, &h);

		viewPortPosistionInPercent.y = (IMGUI_TOP_MENU_HEIGHT + playButtonPanelHeight) / h;
		ImGui::SetNextWindowPos({ viewPortPosistionInPercent.x * w, viewPortPosistionInPercent.y * h });
		ImGui::SetNextWindowSize(ImVec2(viewPortSizeInPercent.x * w, viewPortSizeInPercent.y * h));

		ImGuiWindowFlags windowFlags = 0;

		windowFlags |= ImGuiWindowFlags_NoBackground;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("ViewPort", nullptr, windowFlags);

		// All sizes and positions are dependent on the viewport window size and position
		// The viewports size in percent will remain the same when the window is resized
		viewPortSizeInPercent.x = ImGui::GetWindowSize().x / w;
		viewPortSizeInPercent.y = ImGui::GetWindowSize().y / h;
		viewPortSize.x = ImGui::GetWindowSize().x;
		viewPortSize.y = ImGui::GetWindowSize().y;
		viewPortPosition.x = ImGui::GetWindowPos().x;
		viewPortPosition.y = ImGui::GetWindowPos().y;
		viewPortPosistionInPercent.x = viewPortPosition.x / w;
		viewPortPosistionInPercent.y = viewPortPosition.y / h;


		leftPanelWidth = viewPortPosition.x;
		rightPanelWidth = w - ImGui::GetWindowSize().x - leftPanelWidth;
		bottomPanelHeight = h - ImGui::GetWindowSize().y - viewPortPosition.y;
		playButtonPanelHeight = viewPortPosition.y - IMGUI_TOP_MENU_HEIGHT;

		editorSettings.rendererSettings.width = ImGui::GetWindowSize().x;
		editorSettings.rendererSettings.height = ImGui::GetWindowSize().y;

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

		drawGizmos();

		if (std::dynamic_pointer_cast<GameObject>(selectedObject.lock()))
			drawGizmoOperationsWindow();

		ImGui::End();
	}

	void EditorGUI::drawRightSidePanel()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		int initialPanelWidth = w / 5;
		ImGui::SetNextWindowPos(ImVec2(w - rightPanelWidth, IMGUI_TOP_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(rightPanelWidth, h));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoResize;
		
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

	void EditorGUI::drawGameObject(std::shared_ptr<GameObject> gameObject)
	{
		if (gameObject == nullptr)
			return;

		ImGui::PushID(gameObject->uuid.id.c_str());

		std::string name = gameObject->name;

		if (gameObject->getChildren().size() > 0)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (selectedObject.lock() && (gameObject->getUUID() == selectedObject.lock()->getUUID()))
				flags |= ImGuiTreeNodeFlags_Selected;

			bool open = ImGui::TreeNodeEx(name.c_str(), flags);

			if (ImGui::IsItemClicked())
				selectedObject = gameObject;

			if (open)
			{
				for (auto child : gameObject->getChildren())
					drawGameObject(child);
				
				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Selectable(name.c_str(), selectedObject.lock() && (gameObject->getUUID() == selectedObject.lock()->getUUID()));
			if (ImGui::IsItemClicked())
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

	void EditorGUI::drawLeftSidePanel()
	{
		int w, h;
		window.getWindowSize(&w, &h);
		int panelWidth = w / 5;
		ImGui::SetNextWindowPos(ImVec2(0, IMGUI_TOP_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, h));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("##LeftPanel", nullptr, windowFlags);

		if (ImGui::BeginTabBar("##LeftPanelTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
		{
			if (ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
				ImGui::BeginListBox("##2", ImVec2(500, 500));

				auto rootGameObjects = game->getRootGameObjects();
				std::vector<std::shared_ptr<GameObject>> rootGameObjectsList = std::vector<std::shared_ptr<GameObject>>(rootGameObjects.begin(), rootGameObjects.end());
				std::ranges::sort(rootGameObjectsList, [](std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b) { return a->name < b->name; });

				for (auto gameObject : game->getRootGameObjects())
					drawGameObject(gameObject);

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
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				quitProgram = true;
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
			if (ImGui::MenuItem("Copy", "Ctrl+C"))
			{
				copySelectedGameObject();
			}
			if (ImGui::MenuItem("Paste", "Ctrl+P"))
			{
				pasteGameObject();
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
					cube->addComponent(MeshComponent::createPrimitive(PrimitiveMeshType::CUBE));
					game->addGameObject(cube);
					selectedObject = cube;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					std::shared_ptr<GameObject> sphere = std::make_shared<GameObject>();
					sphere->name = "Sphere";
					sphere->addComponent(MeshComponent::createPrimitive(PrimitiveMeshType::SPHERE));
					game->addGameObject(sphere);
					selectedObject = sphere;
				}
				if (ImGui::MenuItem("Plane"))
				{
					std::shared_ptr<GameObject> plane = std::make_shared<GameObject>();
					plane->name = "Plane";
					plane->addComponent(MeshComponent::createPrimitive(PrimitiveMeshType::PLANE));
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
				ShellExecute(NULL, "open", "https://github.com/hannes44/LIDL-Engine", 0, 0, SW_SHOWDEFAULT);
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
		ImGui::SetNextWindowPos(ImVec2(leftPanelWidth, IMGUI_TOP_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(w - leftPanelWidth - rightPanelWidth, playButtonPanelHeight));

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
			{
				tabFlags = ImGuiTabItemFlags_SetSelected;
				wasStopButtonPressed = false;
			}
				

			if (ImGui::BeginTabItem("Scene", nullptr, tabFlags))
			{
				activeViewPort = ActiveViewPort::Scene;
				ImGui::EndTabItem();
			}

			tabFlags = ImGuiSelectableFlags_None;

			if (wasPlayButtonPressed)
			{
				tabFlags = ImGuiTabItemFlags_SetSelected;
				wasPlayButtonPressed = false;
			}
				

			if (ImGui::BeginTabItem("Game", nullptr, tabFlags))
			{
				activeViewPort = ActiveViewPort::Game;
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::SameLine();
	
		if (sceneState == EditorSceneState::Scene)
		{
			ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];
			ImGui::PushStyleColor(ImGuiCol_Button, color);
		}
		else
		{
			ImVec4 secondaryColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
			ImGui::PushStyleColor(ImGuiCol_Button, secondaryColor);
		}
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = 15;

		float buttonMarginTop = 5;
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - buttonMarginTop);
		if (ImGui::ImageButton("##playIcon", (void*)(intptr_t)playIconTexture->textureIDOpenGL, ImVec2(15, 15), { 0, 1 }, { 1, 0 }))
		{
			if (sceneState == EditorSceneState::Scene)
			{
				sceneState = EditorSceneState::Play;
				wasPlayButtonPressed = true;
				wasStopButtonPressed = false;
				playGame();
			}
			else
			{
				sceneState = EditorSceneState::Scene;
				wasPlayButtonPressed = false;
				wasStopButtonPressed = true;
				stopGame();
			}
		}
		ImGui::PopStyleColor();

		ImGui::End();
	}

	void EditorGUI::drawGizmos()
	{
		// Only draw gizmos in scene view
		if (!editorSettings.showGizmos || activeViewPort != ActiveViewPort::Scene)
			return;

		bool shouldDrawGizmos = false;

		float* modelMatrixPtr = nullptr;
		if (auto lockedSelectedObject = selectedObject.lock())
		{
			if (auto lockedGameObject = dynamic_pointer_cast<GameObject>(lockedSelectedObject))
			{
				shouldDrawGizmos = true;
				modelMatrixPtr = glm::value_ptr(lockedGameObject->transform.transformMatrix);
			}
		}

		if (shouldDrawGizmos)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(viewPortPosition.x, viewPortPosition.y, editorSettings.rendererSettings.width, editorSettings.rendererSettings.height);

			glm::mat4 cameraView = getActiveCamera()->getViewMatrix();

			glm::mat4 projectionMatrix = getActiveCamera()->getProjectionMatrix(editorSettings.rendererSettings.width, editorSettings.rendererSettings.height);

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(projectionMatrix), gizmoOperation, isGizmoOperationInWorldSpace ? ImGuizmo::WORLD : ImGuizmo::LOCAL, modelMatrixPtr);
		}

		isMouseOverGuizmo = ImGuizmo::IsOver();
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

				ImGui::Text("Tag");
				ImGui::SameLine();
				static char gameObjectTagBuffer[255];
				strcpy(gameObjectTagBuffer, lockedGameObject->tag.c_str());
				ImGui::InputText("##GameObjectTagInput", gameObjectTagBuffer, 255);
				lockedGameObject->tag = gameObjectTagBuffer;

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
				if (isAddComponentVisible)
				{
					drawAddComponent();
				}
			}
		}
	}

	void EditorGUI::drawAddComponent()
	{
		ImGui::Text("Add Component");
		ImGui::Separator();

		std::vector<std::string> allComponentNames = { "Box Collider", "Camera", "Mesh", "Physics", "PointLight", "SpotLight", "Sphere Collider", "Controllable" };
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

	void EditorGUI::drawGameSettingsTab()
	{
		ImGui::Text("Application FPS: %.1f", ImGui::GetIO().Framerate);

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Editor Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Rendering Settings");

			drawSerializableVariables(&editorSettings.rendererSettings);

			ImGui::Separator();

			ImGui::Text("Editor Settings");

			drawSerializableVariables(&editorSettings);

			if (editorSettings.useDarkTheme)
			{
				UIHelper.setDarkStyle();
			}
			else if (!editorSettings.useDarkTheme)
			{
				ImGui::StyleColorsLight();
			}

			ImGui::Separator();

			ImGui::Text("Camera Settings");
			ImGui::SliderFloat("Camera Speed", &editorCamera->getComponent<CameraComponent>()->movementSpeed, 0.001f, 1.0f);
			ImGui::SliderFloat("Camera Sensitivity", &editorCamera->getComponent<CameraComponent>()->rotationSpeed, 0.001f, 0.1f);
			ImGui::SliderFloat("Camera FOV", &editorCamera->getComponent<CameraComponent>()->fov, 0.1f, 120.0f);
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

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos(ImVec2(leftPanelWidth, h - bottomPanelHeight));
		ImGui::SetNextWindowSize(ImVec2(w - leftPanelWidth - rightPanelWidth, bottomPanelHeight));
		ImGui::Begin("##BottomPanel", nullptr, windowFlags);

		if (ImGui::BeginTabBar("##BottomTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Assets"))
			{
				if (ImGui::SmallButton("New Folder"))
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
				ImGui::InputText(("##" + seralizableVariable.name).c_str(), name, IM_ARRAYSIZE(name));
				*static_cast<std::string*>(seralizableVariable.data) = name;
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
			else
			{
				LOG_WARN("Cannot serialize variable type: name: {}", seralizableVariable.name);
				return;
			}

			ImGui::SameLine();
			GUIHelper::HelpMarker(seralizableVariable.description.c_str());
		}
	}

	void EditorGUI::drawGizmoOperationsWindow()
	{
		// Only draw gizmos in scene view
		if (!editorSettings.showGizmos || activeViewPort != ActiveViewPort::Scene)
			return;

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;

		float windowWidth = 50;
		float windowHeight = 170;

		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

		ImGuiWindow* operationWindow = ImGui::FindWindowByName("Gizmo Operation");

		// We need to constraint the window to the viewport
		if (operationWindow != nullptr)
		{
			// There is an edge case where both x and y are out of bounds and one of them will be overwritten by the other
			// Saving the initial position of the window to avoid this issue
			int operationWindowPosX = operationWindow->Pos.x;

			if (operationWindow->Pos.x < viewPortPosition.x)
			{
				ImGui::SetNextWindowPos(ImVec2(leftPanelWidth, operationWindow->Pos.y));
				operationWindowPosX = leftPanelWidth;
			}
			else if ((operationWindow->Pos.x + windowWidth) > viewPortPosition.x + viewPortSize.x)
			{
				ImGui::SetNextWindowPos(ImVec2(viewPortPosition.x + viewPortSize.x - windowWidth, operationWindow->Pos.y));
				operationWindowPosX = viewPortPosition.x + viewPortSize.x - windowWidth;
			}

			if (operationWindow->Pos.y < viewPortPosition.y)
				ImGui::SetNextWindowPos(ImVec2(operationWindowPosX, viewPortPosition.y));
			else if ((operationWindow->Pos.y + windowHeight)> viewPortPosition.y + viewPortSize.y)
				ImGui::SetNextWindowPos(ImVec2(operationWindowPosX, viewPortPosition.y + viewPortSize.y - windowHeight));
		}

		ImGui::Begin("Gizmo Operation", nullptr, windowFlags);

		isMouseOverGuizmosOperationWindow = ImGui::IsWindowHovered();

		bool pushedStyleColor = false;
		if (gizmoOperation == ImGuizmo::ROTATE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##rotateOperationButton", (void*)(intptr_t)rotateIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
			gizmoOperation = ImGuizmo::ROTATE;
		
		if (pushedStyleColor)
			ImGui::PopStyleColor();

		pushedStyleColor = false;

		if (gizmoOperation == ImGuizmo::TRANSLATE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##translateOperationButton", (void*)(intptr_t)translateIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
			gizmoOperation = ImGuizmo::TRANSLATE;
		
		if (pushedStyleColor)
			ImGui::PopStyleColor();

		pushedStyleColor = false;

		if (gizmoOperation == ImGuizmo::SCALE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##scaleOperationButton", (void*)(intptr_t)scaleIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
			gizmoOperation = ImGuizmo::SCALE;
		
		if (pushedStyleColor)
			ImGui::PopStyleColor();

		pushedStyleColor = false;

		ImGui::NewLine();

		if (isGizmoOperationInWorldSpace)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			pushedStyleColor = true;
		}

		if (ImGui::ImageButton("##worldOperationButton", (void*)(intptr_t)worldIconTexture->textureIDOpenGL, ImVec2(25, 25), { 0, 1 }, { 1, 0 }))
			isGizmoOperationInWorldSpace = !isGizmoOperationInWorldSpace;
		
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

			// Drag and drop material to game object meshes
			if (auto material = std::dynamic_pointer_cast<Material>(assetNode->asset.lock()))
			{
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Draw the material icon when dragging
					ImGui::Image((void*)(intptr_t)openGLTextureId, ImVec2(30, 30), { 0, 1 }, { 1, 0 });

					glm::vec3 rayDirection = Utils::getMouseRayDirection(window, *getActiveCamera(), viewPortSize, viewPortPosition);
					glm::vec3 rayOrigin = getActiveCamera()->getTransform().getPosition();

					auto gameObjects = Utils::getAABBGameObjectCollisions(game.get(), rayOrigin, rayDirection);
					if (gameObjects.size() > 0)
					{
						if (auto mesh = gameObjects[0]->getComponent<MeshComponent>())
						{
							// Only saving the material on the first frame we drag over the mesh
							if (overwrittenMaterial.expired())
							{
								overwrittenMaterial = mesh->getMaterial();
								mesh->setMaterial(material);
								overwrittenGameObject = gameObjects[0];
							}
							// If we drag the material over to a different mesh, we need to reset the overwritten mesh
							else if (overwrittenGameObject.lock()->uuid.id != gameObjects[0]->uuid.id)
							{
								overwrittenGameObject.lock()->getComponent<MeshComponent>()->setMaterial(overwrittenMaterial.lock());
								overwrittenMaterial.reset();
								overwrittenGameObject.reset();
							}
						}
					}
					else
					{
						// If the mouse is not over a game object, reset the overwritten mesh back to its original material
						if (auto lockedOverWrittenGameObject = overwrittenGameObject.lock())
						{
							lockedOverWrittenGameObject->getComponent<MeshComponent>()->setMaterial(overwrittenMaterial.lock());
						}
						overwrittenMaterial.reset();
						overwrittenGameObject.reset();
					}

					ImGui::EndDragDropSource();
				}

				// When the mouse is released, reset the overwritten model, texture
				if (!ImGui::IsMouseDragging(0))
				{
					overwrittenGameObject.reset();
					overwrittenMaterial.reset();
				}
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

	bool EditorGUI::drawDefaultCheckBox(const std::string& label, bool* value)
	{
		return ImGui::Checkbox(label.c_str(), value);
	}

	void EditorGUI::playGame()
	{
		// Save the current state of the game
		GameSerializer::serializeGame(game.get());
		game->running = true;
		for (auto& [gameObjectId, gameObject] : game->getGameObjects())
		{
			gameObject->initialize();
		}

		sceneState = EditorSceneState::Play;
	}

	void EditorGUI::stopGame()
	{
		game->running = false;
		// Overwrite the current state of the game with the saved state
		GameSerializer::deserializeGame(game.get());

		// We should probably reset the sript states aswell since only serializable script variables will be reset

		sceneState = EditorSceneState::Scene;
	}

	CameraComponent* EditorGUI::getActiveCamera()
	{
		if (activeViewPort == ActiveViewPort::Scene)
		{
			return editorCamera->getComponent<CameraComponent>().get();
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

	void EditorGUI::copySelectedGameObject()
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

	void EditorGUI::pasteGameObject()
	{
		if (auto lockedCopiedGameObject = copiedGameObject.lock())
		{
			LOG_INFO("Pasting game object");
			std::shared_ptr<GameObject> newGameObject = lockedCopiedGameObject->clone();
			game->addGameObject(newGameObject);
			selectedObject = newGameObject;
		}
	}

	bool EditorGUI::isMouseInsideViewPort()
	{
		int mouseX, mouseY;
		window.getMousePosition(&mouseX, &mouseY);

		if (mouseX < viewPortPosition.x || mouseX > (viewPortPosition.x + viewPortSize.x))
			return false;

		if (mouseY < viewPortPosition.y || mouseY >(viewPortPosition.y + viewPortSize.y))
			return false;

		return true;
	}

	void EditorGUI::setupEditorCamera()
	{
		auto editorCameraGameObject = std::make_shared<GameObject>();

		// Create a custom controllable component for the editor camera
		auto editorCameraControllableComponent = std::make_shared<ControllableComponent>();
		editorCameraControllableComponent->setGameObject(editorCameraGameObject.get());
		editorCameraControllableComponent->movementSpeed = 5.f;
		editorCameraControllableComponent->movementType = MovementType::OnHold;
		editorCameraControllableComponent->enableForces = false;
		editorCameraControllableComponent->initialize();

		// Create a camera component for the editor camera
		auto editorCameraComponent = std::make_shared<CameraComponent>();
		auto editorCameraPhysicsComponent = std::make_shared<PhysicsComponent>();
		// Hotfix for camera W/S inversed
		editorCameraPhysicsComponent->forward = glm::vec3(0, 0, -1);

		editorCameraGameObject->addComponent(editorCameraComponent);
		editorCameraGameObject->addComponent(editorCameraPhysicsComponent);
		editorCameraGameObject->addComponent(editorCameraControllableComponent);
		editorCameraGameObject->name = "Editor Camera";
		editorCameraGameObject->transform.setPosition(glm::vec3(0, 7.5f, -20));
		editorCameraGameObject->transform.setRotationFromDirection(glm::vec3(0, 0.5f, -1), glm::vec3(0, 1, 0));

		editorCamera = editorCameraGameObject;

		auto editorGameObjects = std::map<std::string, std::shared_ptr<GameObject>>();
		editorGameObjects[editorCameraGameObject->getUUID().id] = editorCameraGameObject;
		editorPhysicsSettings = GamePhysicsSettings();
		editorPhysicsSettings.enableGravity = false;
		editorPhysicsSettings.enableCollisions = false;
		editorPhysicsSettings.fixedUpdateIntervalMS = 10;
		editorPhysicsSettings.enableFriction = false;

		this->editorGameObjects = std::set<std::shared_ptr<GameObject>>();
		for (auto const& [id, gameObject] : editorGameObjects)
			this->editorGameObjects.insert(gameObject);
	}

	void EditorGUI::createEditorTextures()
	{
		rotateIconTexture = std::shared_ptr<Texture>(Texture::create("rotation_icon.png", false));
		scaleIconTexture = std::shared_ptr<Texture>(Texture::create("scale_icon.png", false));
		translateIconTexture = std::shared_ptr<Texture>(Texture::create("translation_icon.png", false));
		worldIconTexture = std::shared_ptr<Texture>(Texture::create("world_icon.png", false));
		playIconTexture = std::shared_ptr<Texture>(Texture::create("play_icon.png", false));
		viewPortTexture = std::shared_ptr<Texture>(Texture::create());
	}

	void EditorGUI::createEditorInputActions()
	{
		ActionMap::getInstance().addAction("Copy", { Key::LCTRL, Key::C });
		ActionMap::getInstance().addAction("Paste", { Key::LCTRL, Key::V });
		ActionMap::getInstance().addAction("Save", { Key::LCTRL, Key::S });
	}
}
