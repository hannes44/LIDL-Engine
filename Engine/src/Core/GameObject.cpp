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

	void GameObject::addComponent(std::shared_ptr<Component> component) {
		component->setGameObject(this);
		components.push_back(component);
	}
}