#include "SphereColliderComponent.hpp"
#include "Renderer/Renderer.hpp"

namespace engine {
	BoundingBox SphereColliderComponent::getBoundingBox() {
		// The axis aligned bounding box for a sphere is not affected by the sphere's rotation, so we can return the same AABB for all rotations
		return BoundingBox(gameObject->transform.getPosition() + offset, glm::vec3(radius * 2, radius * 2, radius * 2));
	}

	void SphereColliderComponent::drawCollider(CameraComponent* camera) {
		Renderer* renderer = Renderer::getInstance();
		renderer->drawOutlineOfSphere(gameObject->transform.getPosition(), radius, camera);
	}

	std::string SphereColliderComponent::getName() {
		return "Sphere Collider";
	}
}