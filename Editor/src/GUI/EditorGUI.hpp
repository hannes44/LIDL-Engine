#pragma once
#include <Engine.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "../../vendor/ImGuizmo/ImGuizmo.h"
#include "EditorSettings.hpp"
#include "AssetExplorer/AssetManager.hpp"
#include "Project.hpp"
#include "ConsoleGUI.hpp"
#include "UI/GUIHelper.hpp"

#include <WinSock2.h>
#include <mutex>

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

	class EditorGUI : public InputListener, public EventListener
	{
	public:
		EditorGUI(std::shared_ptr<Project> project, EditorSettings& editorSettings);

		void start();

		void renderNewFrame();

		void endFrame();

		void handleInput(const InputEvent& event) override;

		void onEvent(EventType type, std::string message);

		std::shared_ptr<Game> game = nullptr;

		void onMultiplayerStateReceived(std::shared_ptr<Game> game, std::string state);
	private:
		void drawMainMenu();

		void drawViewPort();

		void drawRightSidePanel();

		void drawLeftSidePanel();

		void drawGameObject(std::shared_ptr<GameObject> gameObject);

		void drawTopMenu();

		void drawPlayButtonToolbar();

		void drawGuizmos();

		void drawInspectorSelectedGameObject();

		void ShowAddComponent();

		void drawGameSettingsTab();

		void drawBottomPanel();

		void drawAssetsSection();

		void drawSerializableVariables(Serializable* serializable);

		void drawGuizmoOperationsWindow();

		void drawAssetItem(std::shared_ptr<AssetNode> assetNode);

		void drawCompilationErrorWindow();

		bool defaultCheckBox(const std::string& label, bool* value);

		void playGame();

		void stopGame();

		void changeGame(std::shared_ptr<Game> game);

		Window& window;

		GUIHelper& UIHelper = GUIHelper::getInstance();

		std::weak_ptr<Selectable> selectedObject;

		ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;

		bool isGuizmoOperationInWorldSpace = true;

		std::shared_ptr<GameObject> editorCamera;

		EditorSceneState sceneState = EditorSceneState::Scene;

		ActiveViewPort activeViewPort = ActiveViewPort::Scene;

		// Flag for when the play button is pressed
		bool wasPlayButtonPressed = false;

		bool wasStopButtonPressed = false;

		bool quitProgram = false;

		// In no GUI mode, the gui will not be rendered and the game will accept relative
		bool noGUIMode = false;

		CameraComponent* getActiveCamera();

		EditorSettings editorSettings{};

		std::weak_ptr<GameObject> copiedGameObject;

		std::weak_ptr<AssetNode> selectedAssetNodeFolder;

		std::unique_ptr<AssetManager> assetManager;

		std::shared_ptr<Project> project;

		std::shared_ptr<Texture> rotateIconTexture;

		std::shared_ptr<Texture> translateIconTexture;

		std::shared_ptr<Texture> scaleIconTexture;

		std::shared_ptr<Texture> worldIconTexture;

		std::shared_ptr<Texture> playIconTexture;

		
		void setupMultiplayer(std::shared_ptr<Game> game);
		SOCKET multiplayerSocket = INVALID_SOCKET;
		std::string MULTIPLAYER_STATE_FOLDER = "../../MultiplayerStates/";
		std::string MULTIPLAYER_STATE_FILE_EXTENSION = ".yaml";
		std::mutex multiplayerReceiveLock{};
	};
}