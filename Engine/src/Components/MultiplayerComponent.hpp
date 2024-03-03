#pragma once

#include "Component.hpp"
#include "Core/GameObject.hpp"

namespace engine {

	class MultiplayerComponent : public Component {
	public:
		MultiplayerComponent() {};

		std::string getName() override { return "Multiplayer"; };

		std::shared_ptr<Component> clone() override {
			return std::make_shared<MultiplayerComponent>(*this);
		}
	protected:

	};
}