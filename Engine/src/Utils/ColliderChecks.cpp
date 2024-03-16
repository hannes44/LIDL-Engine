#include "ColliderChecks.hpp"

#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"

namespace engine {
	glm::vec3 getClosestAABBPoint(BoundingBox& box, glm::vec3 point) {
		glm::vec3 closestPoint = glm::vec3(0.0f);
		glm::vec3 min = box.getMin();
		glm::vec3 max = box.getMax();

		closestPoint.x = std::max(min.x, std::min(point.x, max.x));
		closestPoint.y = std::max(min.y, std::min(point.y, max.y));
		closestPoint.z = std::max(min.z, std::min(point.z, max.z));

		return closestPoint;
	}

	// TODO: Implement this correctly, using the actual boxes instead of their AABBs
	// Currently we know that the lazy check already performs a box-box check, so we can just return true.
	bool ColliderChecks::boxBoxCollisionCheck(BoxColliderComponent* first, BoxColliderComponent* second) {
		return true;
	}

	// TODO: Implement this correctly, using the actual box instead of its AABB
	bool ColliderChecks::boxSphereCollisionCheck(BoxColliderComponent* first, SphereColliderComponent* second) {
		glm::vec3 sphereCenter = second->gameObject->getGlobalTransform().getPosition();
		BoundingBox box = first->getBoundingBox();
		glm::vec3 closestPoint = getClosestAABBPoint(box, sphereCenter);
		float distance = glm::length(closestPoint - sphereCenter);

		return distance * distance <= second->radius * second->radius;
	}

	bool ColliderChecks::sphereSphereCollisionCheck(SphereColliderComponent* first, SphereColliderComponent* second) {
		glm::vec3 distance = first->gameObject->getGlobalTransform().getPosition() - second->gameObject->getGlobalTransform().getPosition();
		return glm::length(distance) <= first->radius + second->radius;
	}
}