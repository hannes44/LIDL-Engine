#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
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
		void setRotation(glm::quat rotation);

		glm::vec3 getScale();
		void setScale(glm::vec3 scale);

		glm::mat4 transformMatrix{ 1 };

		void Test() { std::cout << "TEST" << std::endl; };
	};
}
