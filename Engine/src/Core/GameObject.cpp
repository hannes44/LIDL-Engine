#include "GameObject.hpp"
#include "Core/Logger.hpp"
#include <stdexcept>
#include <algorithm>
#include <set>

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
		std::set<std::string> componentNames{};
		for (auto& component : components)
			componentNames.insert(component->getName());

		for (auto& requiredComponentName : component->getRequiredComponents()) {
			if (componentNames.find(requiredComponentName) == componentNames.end()) {
				std::string errorMessage = "Component " + component->getName() + " requires component " + requiredComponentName;
				LOG_FATAL("Fatal error: " + errorMessage);
				throw std::runtime_error(errorMessage);
			}
		}

		component->setGameObject(this);
		components.push_back(component);
	}

	std::shared_ptr<GameObject> GameObject::clone()
	{
		std::shared_ptr<GameObject> newGameObject = std::make_shared<GameObject>(*this);
		newGameObject->uuid = UUID();

		for (auto& component : newGameObject->components) {
			component = component->clone();
			component->gameObject = newGameObject.get();
		}

		return newGameObject;
	}
}