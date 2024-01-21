#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{
	class Transform
	{
	public:
		Transform();

		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	};
}
