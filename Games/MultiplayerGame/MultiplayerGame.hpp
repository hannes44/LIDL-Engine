#pragma once

#include <Engine.hpp>	
#include <vector>
#include <WinSock2.h>

namespace engine {
	class MultiplayerGame : public Game, public InputListener {
	public:
		MultiplayerGame();
		void update() override;
		void initialize() override;

		void handleInput(const InputEvent& event) override;
		bool isMultiplayerGame() override;

		// TODO_MULTIPLAYER: Move to Game.hpp once include issues are resolved
		void setupMultiplayer();
		
	protected:
		double getTargetFrameRate() override;
		GameObject* getRemoteBox();
		void moveRemoteBox();
		void cycleRemoteBoxColour();

		// TODO_MULTIPLAYER: Move this to the Physics Engine once include issues are resolved
		void sendMultiplayerState();
		int multiplayerStateUpdateIntervalMS = 500;
		long long lastMultiplayerStateUpdateTimestamp = 0;

		// TODO_MULTIPLAYER: Move these to Game.hpp once include issues are resolved
		std::string MULTIPLAYER_STATE_FOLDER = "../../MultiplayerStates/";
		std::string MULTIPLAYER_STATE_FILE_EXTENSION = ".yaml";
		SOCKET multiplayerSocket = INVALID_SOCKET;
		void onMultiplayerStateReceived(std::string state);
	};
}