#pragma once

#include "Component.hpp"
#include "CameraComponent.hpp"
#include "Core/BoundingBox.hpp"
#include "glm/glm.hpp"
#include "Core/GameObject.hpp"

namespace engine {

	class ColliderComponent : public Component {
	public:
		friend class GamePhysics;
		bool isColliding(ColliderComponent* other);
		virtual void drawCollider(CameraComponent* camera) = 0;
		virtual BoundingBox getBoundingBox() = 0;
		ColliderComponent(glm::vec3 offset, glm::vec3 extent, bool isPlatform = false) : offset(offset), extent(extent), isPlatform(isPlatform) {}

		void subscribeToCollision(std::function<void(Game*, GameObject*, ColliderComponent*)> callback) { onCollision = callback; }

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::VECTOR3, "Offset", "The offset of the collider", &offset},
				{SerializableType::VECTOR3, "Extent", "The extent of the collider", &extent},
				{SerializableType::BOOLEAN, "IsPlatform", "Whether the collider is a platform", &isPlatform}
			};
		};

	protected:
		bool deepCollisionCheck(ColliderComponent* other);

		static void drawBoundingBox(BoundingBox& box, CameraComponent* camera, bool isColliding = false);

		glm::vec3 offset{ 0 };
		glm::vec3 extent{ 0 };
		bool isPlatform = false;

		bool isCurrentlyColliding = false;
		std::function<void(Game*, GameObject*, ColliderComponent*)> onCollision;
	};
}