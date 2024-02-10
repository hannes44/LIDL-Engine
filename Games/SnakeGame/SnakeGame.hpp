#include <Engine.hpp>	
#include <vector>

namespace engine {
	class SnakeGame : public Game, public InputListener {
	public:
		SnakeGame();
		void update() override;
		void initialize() override;

		void handleInput(const InputEvent& event) override;

		std::vector<std::weak_ptr<GameObject>> snake;
		std::weak_ptr<GameObject> apple;
		void consumeApple();
		void gameOver();
	protected:
		void addBody();
		void moveAppleToRandomPosition();
		glm::vec3 getTailDirection();
		double getTargetFrameRate() override;
		glm::vec3 direction{};
		float bodyGap = 0.7f;
	};
}