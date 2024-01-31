#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "Window.hpp"
#include "Game.hpp"
#include "../vendor/ImGuizmo/ImGuizmo.h"
#include "RendererSettings.hpp"
#include "EditorSettings.hpp"
#include "AssetManager.hpp"
#include "Project.hpp"

namespace engine
{
	enum class EditorSceneState
	{
		Scene, // While in scene state, run no game logic
		Play,  // While in play state, run all game logic
		Pause  // While in pause state, pause the active game loop
	};

	enum class ActiveViewPort
	{
		Scene,
		Game
	};

	class EditorGUI
	{
	public:
		EditorGUI(std::shared_ptr<Project> project);

		void start();

		void renderNewFrame();

		void endFrame();
		
		std::shared_ptr<Game> game = nullptr;
	private:
		void drawMainMenu();

		void drawViewPort();

		void drawRightSidePanel();

		void drawLeftSidePanel();

		void drawTopMenu();

		void drawPlayButtonToolbar();

		void drawGuizmos();

		void drawInspectorSelectedGameObject();

		void drawGameSettingsTab();	

		void drawBottomPanel();

		void drawAssetsSection();

		void drawAssetItem(std::shared_ptr<AssetNode> assetNode);

		bool defaultCheckBox(const std::string& label, bool* value);

		void changeGame(std::shared_ptr<Game> game);

		Window& window;

		std::weak_ptr<Selectable> selectedObject;

		ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;

		Camera editorCamera{};

		EditorSceneState sceneState = EditorSceneState::Scene;

		ActiveViewPort activeViewPort = ActiveViewPort::Scene;

		Camera* getActiveCamera();

		EditorSettings editorSettings{};

		std::weak_ptr<AssetNode> selectedAssetNodeFolder;

		std::unique_ptr<AssetManager> assetManager;

		std::shared_ptr<Project> project;
	};
}