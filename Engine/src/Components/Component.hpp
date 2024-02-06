#pragma once

#include "Core/GameObject.hpp"

#include <memory>
#include <string>
#include <vector>
#include "Serializer/Serializable.hpp"

namespace engine
{
	// Forward declaration due to circular dependency
	class GameObject;

	// Component class for Entity-Component-System
	// All components must inherit from this class
	// A future improvement would be to use a component library such as EnTT
	// to improve performance caused by bad memory locality
	class Component : public Serializable
	{
	public:
		virtual void initialize() {};
		virtual void update() {};

		virtual std::string getName() = 0;

		virtual std::string getDescription() { return ""; };

		void setGameObject(GameObject* gameObject) {
			this->gameObject = gameObject;
		}

	protected:
		GameObject* gameObject = nullptr;
	};
}
