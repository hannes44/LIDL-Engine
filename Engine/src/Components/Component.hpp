#pragma once

#include "Core/GameObject.hpp"

#include <memory>
#include <string>
#include <vector>
#include "Serializer/Serializable.hpp"
#include <set>
#include "Core/UUID.hpp"

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
		virtual void initialize(){};
		virtual void update(){};

		virtual std::string getName() = 0;

		virtual std::shared_ptr<Component> clone() = 0;

		virtual std::set<std::string> getRequiredComponents() { return {}; };

		virtual std::string getDescription() { return ""; };

		UUID uuid{};

		void setGameObject(GameObject *gameObject)
		{
			this->gameObject = gameObject;
		}

		GameObject *gameObject = nullptr;
	};
}
