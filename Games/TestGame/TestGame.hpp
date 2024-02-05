#include <Engine.hpp>	
#include <vector>

namespace engine {
	class TestGame : public Game {
	public:
		TestGame();
		void update() override;
		void initialize() override;

	protected:
		double getTargetFrameRate() override;
	};
}