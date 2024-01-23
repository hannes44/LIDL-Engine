#pragma once

#include <glm/glm.hpp>

namespace engine
{
	class Camera
	{
	public:
		Camera();

		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);

		glm::mat4 getTransform();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		glm::vec3 translation {0, 0, 0};
		glm::vec3 rotation {0, 0, 0};
		glm::vec3 direction {-1, -1, -1};
		
		float fov = 90;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		float cameraSpeed = 10.f;
	};
}