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
		
	protected:
		double getTargetFrameRate() override;
		GameObject* getRemoteBox();
		void moveRemoteBox();
		void cycleRemoteBoxColour();
	};
}