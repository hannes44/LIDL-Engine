#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace engine
{
	class Transform
	{
	public:
		Transform();

		glm::vec3 getPosition();
		void setPosition(glm::vec3 position);
		void shiftPosition(glm::vec3 offset);

		glm::quat getRotation();
		void setRotationFromDirection(glm::vec3 direction, glm::vec3 normal);
		void setRotationFromQuaternion(const glm::quat& orientation);

		glm::vec3 getScale();
		void setScale(glm::vec3 scale);

		glm::vec3 getLocalForward();

		glm::mat4 transformMatrix{ 1 };
	};
}
