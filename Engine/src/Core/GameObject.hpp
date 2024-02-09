#pragma once

#include "Transform.hpp"
#include "Components/Component.hpp"
#include <vector>
#include <memory>
#include <string>
#include "Uuid.hpp"
#include "Selectable.hpp"

namespace engine
{
	// Forward declarations due to circular dependency
	class Component;
	class Game;

	class GameObject : public Selectable
	{
	public:
		GameObject() = default;
		// Called every frame

		void update();

		// Called at initialization
		void initialize();

		Transform transform;

		bool isVisible = true;

		std::string name = "GameObject";

		UUID uuid{};

		UUID getUUID() override;

		Game* game = nullptr;

		void addComponent(std::shared_ptr<Component> component);
		std::vector<std::shared_ptr<Component>>& getComponents() {
			return components;
		}

		template<typename T> std::shared_ptr<T> getComponent() {
			for (auto& component : components) {
				if (std::dynamic_pointer_cast<T>(component)) {
					return std::dynamic_pointer_cast<T>(component);
				}
			}

			return nullptr;
		}

		template<typename T> bool hasComponent() {
			return getComponent<T>() != nullptr;
		}

		std::string getName() override { return name; };

	private:
		// TODO: Should limit each component to one of each type
		std::vector<std::shared_ptr<Component>> components{};
	};
}
