#include "SphereColliderComponent.hpp"
#include "Renderer/Renderer.hpp"

namespace engine {
    BoundingBox SphereColliderComponent::getBoundingBox() {
        return BoundingBox(gameObject->transform.getPosition() + offset, extent);
    }

    void SphereColliderComponent::drawCollider(Camera* camera) {
        drawBoundingBox(this, getBoundingBox(), camera);
    }

    std::string SphereColliderComponent::getName() {
        return "SphereColliderComponent";
    }
}