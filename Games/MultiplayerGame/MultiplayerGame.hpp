#pragma once

#include <Engine.hpp>	
#include <vector>

namespace engine {
	class MultiplayerGame : public Game, public InputListener {
	public:
		MultiplayerGame();
		void update() override;
		void initialize() override;

		void onInput(const InputEvent& event) override;
		bool isMultiplayerGame() override;
		
	protected:
		double getTargetFrameRate() override;
		GameObject* getRemoteBox();
		void moveRemoteBox();
		void cycleRemoteBoxColour();
	};
}