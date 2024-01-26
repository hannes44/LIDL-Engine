#include "Game.hpp"
#include "GameObject.hpp"
#include <vector>
#include "EditorGUI.hpp"

namespace engine {
	class TestGame : public Game {
	public:
		void update() override;
		void initialize() override;

	protected:
		double getTargetFrameRate() override;
		std::vector<GameObject*> gameObjects{};

		EditorGUI gui {};
	};
}