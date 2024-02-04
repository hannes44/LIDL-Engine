#include "BoundingBox.hpp"

namespace engine {
    bool BoundingBox::isPointInside(glm::vec3 point) {
        return (
            point.x >= minX() &&
            point.x <= maxX() &&
            
            point.y >= minY() &&
            point.y <= maxY() &&
            
            point.z >= minZ() &&
            point.z <= maxZ() / 2
        );
    }
    
    bool BoundingBox::isIntersecting(BoundingBox* other) {
        return (
            minX() <= other->maxX() &&
            maxX() >= other->minX() &&

            minY() <= other->maxY() &&
            maxY() >= other->minY() &&

            minZ() <= other->maxZ() &&
            maxZ() >= other->minZ()
        );
    }
}