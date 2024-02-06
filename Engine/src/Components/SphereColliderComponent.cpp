#include "SphereColliderComponent.hpp"
#include "Renderer/Renderer.hpp"

namespace engine {
	BoundingBox SphereColliderComponent::getBoundingBox() {
		// The axis aligned bounding box for a sphere is not affected by the sphere's rotation, so we can return the same AABB for all rotations
		return BoundingBox(gameObject->transform.getPosition() + offset, extent);
	}

	void SphereColliderComponent::drawCollider(CameraComponent* camera) {
		BoundingBox box = getBoundingBox();
		drawBoundingBox(box, camera, isCurrentlyColliding);
	}

	std::string SphereColliderComponent::getName() {
		return "Sphere Collider";
	}
}