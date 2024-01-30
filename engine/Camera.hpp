#pragma once

#include <glm/glm.hpp>

namespace engine
{
	// Could add the camera as a component to allow for multiple cameras and more control for the game creator
	class Camera
	{
	public:
		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);

		glm::mat4 getTransform();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		glm::vec3 translation {20, 20, 20};
		glm::vec3 rotation {90, 0, 0};
		glm::vec3 direction {-1, -1, -1};
		
		float fov = 50;
		float nearPlane = 0.1;
		float farPlane = 1000;
		float cameraSpeed = 100;
	};
}