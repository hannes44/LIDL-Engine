#include <Engine.hpp>	
#include <vector>

namespace engine {
	class SnakeGame : public Game {
	public:
		SnakeGame();
		void update() override;
		void initialize() override;
	protected:
		double getTargetFrameRate() override;
	};
}