#include "Game.hpp"

namespace engine {
	class TestGame : public Game {
	public:
		void update() override;
		void initialize() override;

	protected:
		double getTargetFrameRate() override;
	};
}