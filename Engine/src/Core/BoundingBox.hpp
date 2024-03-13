#pragma once

#include "glm/glm.hpp"

namespace engine {
	class BoundingBox {
	public:
		BoundingBox(glm::vec3 center, glm::vec3 extent) : center(center), extent(extent) {}

		bool isPointInside(glm::vec3 point);
		bool isIntersecting(BoundingBox* other);
		bool isRayIntersecting(glm::vec3 rayOrigin, glm::vec3 rayDirection);

		glm::vec3 getCenter() { return center; }
		glm::vec3 getExtent() { return extent; }

		glm::vec3 getMin() { return center - extent / 2.0f; }
		glm::vec3 getMax() { return center + extent / 2.0f; }

	protected:
		glm::vec3 center{ 0 };
		glm::vec3 extent{ 0 };

		float minX() { return center.x - extent.x / 2; }
		float maxX() { return center.x + extent.x / 2; }

		float minY() { return center.y - extent.y / 2; }
		float maxY() { return center.y + extent.y / 2; }

		float minZ() { return center.z - extent.z / 2; }
		float maxZ() { return center.z + extent.z / 2; }
	};
}