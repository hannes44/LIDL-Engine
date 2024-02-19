#include <Engine.hpp>	
#include <vector>

namespace engine {
	class FPSGame : public Game {
	public:
		FPSGame();
		void update() override;
		void initialize() override;
	protected:
		double getTargetFrameRate() override;
	};
}