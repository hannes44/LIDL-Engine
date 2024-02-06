#define GLM_ENABLE_EXPERIMENTAL

#include "BoxColliderComponent.hpp"
#include "Renderer/Renderer.hpp"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>


namespace engine {
	BoundingBox BoxColliderComponent::getBoundingBox() {
		cornerExtents = {
			glm::vec3(extent.x, extent.y, extent.z),
			glm::vec3(extent.x, extent.y, -extent.z),
			glm::vec3(extent.x, -extent.y, extent.z),
			glm::vec3(extent.x, -extent.y, -extent.z),
			glm::vec3(-extent.x, extent.y, extent.z),
			glm::vec3(-extent.x, extent.y, -extent.z),
			glm::vec3(-extent.x, -extent.y, extent.z),
			glm::vec3(-extent.x, -extent.y, -extent.z)
		};

		const glm::quat rotation = gameObject->transform.getRotation();
		glm::vec3 maxPoints = glm::vec3();

		for (auto& cornerOffset : cornerExtents) {
			glm::vec3 corner = glm::rotate(rotation, cornerOffset);
			
			maxPoints.x = std::max(maxPoints.x, corner.x);
			maxPoints.y = std::max(maxPoints.y, corner.y);
			maxPoints.z = std::max(maxPoints.z, corner.z);
		}

		return BoundingBox(gameObject->transform.getPosition() + offset, maxPoints);
	}

	void BoxColliderComponent::drawCollider(CameraComponent* camera) {
		BoundingBox box = getBoundingBox();
		drawBoundingBox(box, camera, isCurrentlyColliding);
	}

	std::string BoxColliderComponent::getName() {
		return "Box Collider";
	}
}