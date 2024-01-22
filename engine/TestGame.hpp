#include "Game.hpp"

namespace engine {
	class TestGame : public Game {
	public:
		void update() override;
		void initialize() override;
		~TestGame() {};

	protected:
		double getTargetFrameRate() override;
	};
}