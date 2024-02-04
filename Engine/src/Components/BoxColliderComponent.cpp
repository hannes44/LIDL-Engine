#include "BoxColliderComponent.hpp"
#include "Renderer/Renderer.hpp"

namespace engine {
    BoundingBox BoxColliderComponent::getBoundingBox() {
        return BoundingBox(gameObject->transform.getPosition() + offset, extent);
    }

    void BoxColliderComponent::drawCollider(Camera* camera) {
        drawBoundingBox(this, getBoundingBox(), camera);
    }

    std::string BoxColliderComponent::getName() {
        return "BoxColliderComponent";
    }
}