#pragma once

#include <Engine.hpp>	
#include <vector>

namespace engine {
	class MultiplayerGame : public Game, public InputListener {
	public:
		MultiplayerGame();
		void update() override;
		void initialize() override;

		void handleInput(const InputEvent& event) override;

		
	protected:
		double getTargetFrameRate() override;
		GameObject* getRemoteBox();
		void moveRemoteBox();
		void cycleRemoteBoxColour();

		void onMessage(std::string msg);


		std::string MULTIPLAYER_STATE_FOLDER = "../../MultiplayerStates/";
	};
}