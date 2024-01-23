#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{
	class Transform
	{
	public:
		Transform();

		glm::vec3 getPosition();
		void setPosition(glm::vec3 position);

		glm::vec3 getRotation();
		void setRotation(glm::vec3 rotation);

		glm::vec3 getScale();
		void setScale(glm::vec3 scale);

		glm::mat4 transformMatrix{ 1 };
	};
}
