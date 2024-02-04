#include "BoxColliderComponent.hpp"
#include "Renderer/Renderer.hpp"

namespace engine {
	BoundingBox BoxColliderComponent::getBoundingBox() {
		return BoundingBox(gameObject->transform.getPosition() + offset, extent);
	}

	void BoxColliderComponent::drawCollider(CameraComponent* camera) {
		BoundingBox box = getBoundingBox();
		drawBoundingBox(this, box, camera);
	}

	std::string BoxColliderComponent::getName() {
		return "Box Collider";
	}
}