#pragma once

#include "Component.hpp"
#include "CameraComponent.hpp"
#include "Core/BoundingBox.hpp"
#include "glm/glm.hpp"

namespace engine {

    class ColliderComponent : public Component {
    public:
        bool isColliding(ColliderComponent* other);
        virtual void drawCollider(CameraComponent* camera) = 0;
        virtual BoundingBox getBoundingBox() = 0;
        ColliderComponent(glm::vec3 offset, glm::vec3 extent) : offset(offset), extent(extent) {}
    protected:
        bool deepCollisionCheck(ColliderComponent* other);

        static void drawBoundingBox(BoundingBox& box, CameraComponent* camera);
        
        glm::vec3 offset{0};
        glm::vec3 extent{0};
    };
}