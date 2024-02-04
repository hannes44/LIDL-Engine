#pragma once

#include "glm/glm.hpp"

namespace engine {
    class BoundingBox {
    public:
        BoundingBox(glm::vec3 center, glm::vec3 extent) : center(center), extent(extent) {}
        
        bool isPointInside(glm::vec3 point);
        bool isIntersecting(BoundingBox* other);

        glm::vec3 getCenter() { return center; }
        glm::vec3 getExtent() { return extent; }

    protected:
        glm::vec3 center{0};
        glm::vec3 extent{0};

        float minX(){ return center.x - extent.x / 2; }
        float maxX(){ return center.x + extent.x / 2; }

        float minY(){ return center.y - extent.y / 2; }
        float maxY(){ return center.y + extent.y / 2; }

        float minZ(){ return center.z - extent.z / 2; }
        float maxZ(){ return center.z + extent.z / 2; }
    };
}