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

	Transform GameObject::getGlobalTransform() {
		Transform globalTransform = transform;
		std::shared_ptr<GameObject> currentParent = parent;
		while (currentParent) {
			// TODO: Update the rotation as well
			globalTransform.shiftPosition(currentParent->transform.getPosition());

			currentParent = currentParent->parent;
		}

		return globalTransform;
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

	GameObject GameObject::clone()
	{
		GameObject newGameObject = *this;
		newGameObject.uuid = UUID();
		return newGameObject;
	}
}