#include "Utils/ColliderChecks.hpp"
#include "ColliderComponent.hpp"
#include "BoxColliderComponent.hpp"
#include "SphereColliderComponent.hpp"
#include "Renderer/Renderer.hpp"
#include "CameraComponent.hpp"

namespace engine {
    void ColliderComponent::drawBoundingBox(ColliderComponent* collider, BoundingBox& box, CameraComponent* camera) {

        // The center position of the box
        glm::vec3 centerPos = collider->gameObject->transform.getPosition() + collider->offset;

        // The start point, top corner of the box
        glm::vec3 currPos = centerPos + glm::vec3(-collider->extent.x / 2, collider->extent.y / 2, -collider->extent.z / 2);
        
        // The end point of the line
        glm::vec3 nextPos = currPos + glm::vec3(collider->extent.x, 0, 0);

        // We will move on the top surface of the box, so we use this offset vector to move to the mirrored bottom point
        glm::vec3 down = glm::vec3(0, -collider->extent.y, 0);
        
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

        // The offsets that take us to all the top surface corners of the box
        std::vector<glm::vec3> nextPointOffsets = {
            glm::vec3(0, 0, collider->extent.z),
            glm::vec3(-collider->extent.x, 0, 0),
            glm::vec3(0, 0, -collider->extent.z),
            glm::vec3(0, 0, 0) // Needed for the last rendering pass, but the actual value is never used
        };
        
        Renderer* renderer = Renderer::getInstance();
        for(auto nextPointOffset : nextPointOffsets) {
            renderer->drawLine(currPos, nextPos, color, camera); // Top line
            renderer->drawLine(currPos + down, nextPos + down, color, camera); // Bottom line (mirrored from top)
            renderer->drawLine(currPos, currPos + down, color, camera); // Corner, vertically linking top and bottom

            // Update the current position
            currPos = nextPos;

            // Move to the next position
            nextPos += nextPointOffset;
        }
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
        
        if(firstBox && secondBox) {
            return ColliderChecks::boxBoxCollisionCheck(firstBox, secondBox);
        }
        else if(firstBox && secondSphere) {
            return ColliderChecks::boxSphereCollisionCheck(firstBox, secondSphere);
        }
        else if(firstSphere && secondBox) {
            return ColliderChecks::boxSphereCollisionCheck(secondBox, firstSphere);
        }

        return false;
    }
}