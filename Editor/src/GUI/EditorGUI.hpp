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

		void onInput(const InputEvent& event) override;
		void onEvent(EventType type, std::string message);
		void onMultiplayerStateReceived(std::shared_ptr<Game> game, std::string state);
	private:
		void renderNewFrame();
		void endFrame();

		void drawMainMenu();
		void drawViewPort();
		void drawRightSidePanel();
		void drawLeftSidePanel();
		void drawGameObject(std::shared_ptr<GameObject> gameObject);
		void drawTopMenu();
		void drawPlayButtonToolbar();
		void drawGizmos();
		void drawInspectorSelectedGameObject();
		void drawAddComponent();
		void drawGameSettingsTab();
		void drawBottomPanel();
		void drawAssetsSection();
		void drawSerializableVariables(Serializable* serializable);
		void drawGizmoOperationsWindow();
		void drawAssetItem(std::shared_ptr<AssetNode> assetNode);
		void drawCompilationErrorWindow();
		bool drawDefaultCheckBox(const std::string& label, bool* value);

		void playGame();
		void stopGame();
		void changeGame(std::shared_ptr<Game> game);

		void copySelectedGameObject();
		void pasteGameObject();

		bool isMouseInsideViewPort();

		// The editor camera and its physics settings are set up here
		void setupEditorCamera();

		void createEditorTextures();
		void createEditorInputActions();

		void setupMultiplayer(std::shared_ptr<Game> game);

		CameraComponent* getActiveCamera();

		std::shared_ptr<Game> game = nullptr;

		Window& window;
		GUIHelper& UIHelper = GUIHelper::getInstance();
		std::unique_ptr<AssetManager> assetManager;
		std::shared_ptr<Project> project;

		std::weak_ptr<Selectable> selectedObject;
		std::shared_ptr<GameObject> editorCamera;

		// Editor states
		ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
		EditorSceneState sceneState = EditorSceneState::Scene;
		ActiveViewPort activeViewPort = ActiveViewPort::Scene;

		// The game objects that are only visible in the editor
		std::set<std::shared_ptr<GameObject>> editorGameObjects;

		// Physics settings for the editor camera
		GamePhysicsSettings editorPhysicsSettings;
		EditorSettings editorSettings{};

		bool wasPlayButtonPressed = false;		// Flag for when the play button is pressed
		bool wasStopButtonPressed = false;
		bool quitProgram = false;
		bool noGUIMode = false; // In no GUI mode, the gui will not be rendered and the game will accept relative
		bool isMouseOverGuizmo = false;
		bool isMouseOverGuizmosOperationWindow = false;
		bool isGizmoOperationInWorldSpace = true;

		// When a material is dragged from the asset explorer over a mesh component, this will be the material that is being overwritten
		// If the mouse is then dragged away from the mesh component, the material will be reset to its original value
		std::weak_ptr<Material> overwrittenMaterial;
		std::weak_ptr<GameObject> overwrittenGameObject;
		std::weak_ptr<GameObject> copiedGameObject;
		std::weak_ptr<AssetNode> selectedAssetNodeFolder;
		std::shared_ptr<Material> temporaryTextureMaterial = std::make_shared<Material>(); // Temporary material created to display drag and drop texture preview

		// Gizmos operation menu icons
		std::shared_ptr<Texture> rotateIconTexture;
		std::shared_ptr<Texture> translateIconTexture;
		std::shared_ptr<Texture> scaleIconTexture;
		std::shared_ptr<Texture> worldIconTexture;

		std::shared_ptr<Texture> playIconTexture;

		// The framebuffer that the game will be rendered into
		std::shared_ptr<Texture> viewPortTexture;

		// These widths will change dynamically when the user resizes the panels
		float leftPanelWidth = 300;
		float rightPanelWidth = 300;
		float bottomPanelHeight = 300;
		float playButtonPanelHeight = 30;

		glm::vec2 viewPortSize = glm::vec2(500, 500);
		glm::vec2 viewPortSizeInPercent = glm::vec2(0.5f, 0.5f);
		glm::vec2 viewPortPosition = glm::vec2(0, 0);
		glm::vec2 viewPortPosistionInPercent = glm::vec2(0.25f, 0.25f);

		SOCKET multiplayerSocket = INVALID_SOCKET;
		std::string MULTIPLAYER_STATE_FOLDER = "../../../Multiplayer/MultiplayerStates/";
		std::string MULTIPLAYER_STATE_FILE_EXTENSION = ".yaml";
		std::mutex multiplayerReceiveLock{};
	};
}