#include "Utils/ColliderChecks.hpp"
#include "ColliderComponent.hpp"
#include "BoxColliderComponent.hpp"
#include "SphereColliderComponent.hpp"
#include "Renderer/Renderer.hpp"
#include "CameraComponent.hpp"
#include "Utils/Utils.hpp"

namespace engine {
	void ColliderComponent::drawColliderAABB(CameraComponent* camera, bool isColliding) {
		BoundingBox box = getBoundingBox();
		Utils::drawBoundingBox(box, camera, isColliding ? colors.aabbColliding : colors.aabbNotColliding);
	}

	bool ColliderComponent::isColliding(ColliderComponent* other) {
		BoundingBox otherBoundingBox = other->getBoundingBox();
		// For performance reasons, first check whether the bounding boxes are intersecting, since the collider is contained within its bounding box
		bool lazyCheck = getBoundingBox().isIntersecting(&otherBoundingBox);
		if (!lazyCheck)
			return false;

		// If the bounding boxes are intersecting, perform a deep collision check to check whether the colliders themselves are intersecting
		return deepCollisionCheck(other);
	}

	bool ColliderComponent::deepCollisionCheck(ColliderComponent* other) {
		auto firstBox = dynamic_cast<BoxColliderComponent*>(this);
		auto secondBox = dynamic_cast<BoxColliderComponent*>(other);

		auto firstSphere = dynamic_cast<SphereColliderComponent*>(this);
		auto secondSphere = dynamic_cast<SphereColliderComponent*>(other);

		if (firstBox && secondBox) {
			return ColliderChecks::boxBoxCollisionCheck(firstBox, secondBox);
		}
		else if (firstBox && secondSphere) {
			return ColliderChecks::boxSphereCollisionCheck(firstBox, secondSphere);
		}
		else if (firstSphere && secondBox) {
			return ColliderChecks::boxSphereCollisionCheck(secondBox, firstSphere);
		}

		return false;
	}
}