#pragma once

#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"

namespace engine {

    class ColliderChecks {
    public:
        static bool boxBoxCollisionCheck(BoxColliderComponent* first, BoxColliderComponent* second);
        static bool boxSphereCollisionCheck(BoxColliderComponent* box, SphereColliderComponent* sphere);
    };
}