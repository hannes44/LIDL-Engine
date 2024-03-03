#pragma once

#include "ColliderComponent.hpp"
#include "Core/BoundingBox.hpp"
#include "CameraComponent.hpp"

namespace engine {

	class SphereColliderComponent : public ColliderComponent {
	public:
		void drawCollider(CameraComponent* camera);
		BoundingBox getBoundingBox();
		SphereColliderComponent(glm::vec3 offset = glm::vec3(0, 0, 0), glm::vec3 extent = glm::vec3(1, 1, 1), bool isPlatform = false) : ColliderComponent(offset, extent, isPlatform) {}

		std::string getName() override;

	protected:

	};
}