#pragma once

#include <glm/glm.hpp>

namespace engine
{

	// Could add the camera as a component to allow for multiple cameras and more control for the game creator
	class Camera
	{
	public:
		Camera();

		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);

		glm::mat4 getTransform();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		glm::vec3 translation {30, 30, 30};
		glm::vec3 rotation {0, 0, 0};
		glm::vec3 direction {-1, -1, -1};
		
		float fov = 50;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		float cameraSpeed = 100.f;
	};
}