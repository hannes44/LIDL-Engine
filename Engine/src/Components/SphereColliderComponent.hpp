#pragma once

#include "ColliderComponent.hpp"
#include "Core/BoundingBox.hpp"
#include "CameraComponent.hpp"

namespace engine {

	class SphereColliderComponent : public ColliderComponent {
	public:
		void drawCollider(CameraComponent* camera);
		BoundingBox getBoundingBox();
		SphereColliderComponent(glm::vec3 offset = glm::vec3(0, 0, 0), glm::vec3 extent = glm::vec3(1, 1, 1)) : ColliderComponent(offset, extent) {}

		std::string getName() override;

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::VECTOR3, "Offset", "The offset of the collider", &offset},
				{SerializableType::VECTOR3, "Extent", "The extent of the collider", &extent}
			};
		};

	protected:

	};
}