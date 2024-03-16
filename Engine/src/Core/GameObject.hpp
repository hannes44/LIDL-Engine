#pragma once

#include "Transform.hpp"
#include "Components/Component.hpp"
#include <vector>
#include <memory>
#include <string>
#include <set>
#include "Uuid.hpp"
#include "Selectable.hpp"

namespace engine
{
	// Forward declarations due to circular dependency
	class Component;
	class Game;

	class GameObject : public Selectable
	{
		friend class Game;

	public:
		GameObject() = default;

		// Called every frame
		void update(float deltaTime);

		// Called at initialization
		void initialize();

		Transform getGlobalTransform();

		UUID getUUID() override;

		std::set<std::shared_ptr<GameObject>> getChildren() { return children; };
		std::shared_ptr<GameObject> getParent() { return parent; };

		std::vector<std::shared_ptr<Component>> &getComponents()
		{
			return components;
		}

		template <typename T>
		std::shared_ptr<T> getComponent()
		{
			for (auto &component : components)
			{
				if (std::dynamic_pointer_cast<T>(component))
					return std::dynamic_pointer_cast<T>(component);
			}

			return nullptr;
		}

		template <typename T>
		bool hasComponent() { return getComponent<T>() != nullptr; }
		void addComponent(std::shared_ptr<Component> component);

		std::string getName() override { return name; };

		std::shared_ptr<GameObject> clone();

		std::string name = "GameObject";
		std::string tag = "";

		Game* game = nullptr;
		UUID uuid{};
		Transform transform;

		bool isExternalMultiplayerObject = false;
		bool isVisible = true;

	private:
		// TODO: Should limit each component to one of each type
		std::vector<std::shared_ptr<Component>> components{};
		std::set<std::shared_ptr<GameObject>> children{};
		
		bool added = false;

		std::shared_ptr<GameObject> parent = nullptr;

	};
}
