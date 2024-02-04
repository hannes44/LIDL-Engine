#pragma once

#include "ColliderComponent.hpp"
#include "Core/BoundingBox.hpp"
#include "Core/Camera.hpp"

namespace engine {

    class BoxColliderComponent : public ColliderComponent {
    public:
        void drawCollider(Camera* camera);
        BoundingBox getBoundingBox();
        BoxColliderComponent(glm::vec3 offset, glm::vec3 extent) : ColliderComponent(offset, extent) {}

        std::string getName() override;

    protected:

    };
}