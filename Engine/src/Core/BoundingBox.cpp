#include "BoundingBox.hpp"
#include "Utils/Utils.hpp"

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

	bool BoundingBox::isRayIntersecting(glm::vec3 rayOrigin, glm::vec3 rayDirection)
	{
		glm::vec3 tMin = (getMin() - rayOrigin) / rayDirection;
		glm::vec3 tMax = (getMax() - rayOrigin) / rayDirection;

		glm::vec3 t1 = Utils::vec3min(tMin, tMax);
		glm::vec3 t2 = Utils::vec3max(tMin, tMax);

		float tNear = std::max(std::max(t1.x, t1.y), t1.z);
		float tFar = std::min(std::min(t2.x, t2.y), t2.z);

		return tNear >= 0 && tNear < tFar;
	}
}