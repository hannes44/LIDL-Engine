#include "SphereColliderComponent.hpp"
#include "Renderer/Renderer.hpp"

namespace engine {
	BoundingBox SphereColliderComponent::getBoundingBox() {
		return BoundingBox(gameObject->transform.getPosition() + offset, extent);
	}

	void SphereColliderComponent::drawCollider(CameraComponent* camera) {
		BoundingBox box = getBoundingBox();
		drawBoundingBox(this, box, camera);
	}

	std::string SphereColliderComponent::getName() {
		return "Sphere Collider";
	}
}