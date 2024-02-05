#include <Engine.hpp>	
#include <vector>

namespace engine {
	class TestGame2 : public Game {
	public:
		TestGame2();
		void update() override;
		void initialize() override;

	protected:
		double getTargetFrameRate() override;
	};
}