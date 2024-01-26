#pragma once
#include "Camera.hpp"

namespace engine
{
	enum class EditorSceneState
	{
		Scene, // While in scene state, run no game logic
		Play,  // While in play state, run all game logic
		Pause  // While in pause state, pause the active game loop
	};

	class Editor
	{
	public:
		Editor();

		void start();

		Camera editorCamera{};

		EditorSceneState sceneState = EditorSceneState::Scene;
	};
}