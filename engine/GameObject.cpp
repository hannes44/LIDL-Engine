#include "GameObject.hpp"

namespace engine {

	void GameObject::initialize() {
		for (auto& component : components) {
			component->initialize();
		}
	}

	UUID GameObject::getUUID()
	{
		return uuid;
	}

	void GameObject::update() {
		for (auto& component : components) {
			component->update();
		}
	}
}