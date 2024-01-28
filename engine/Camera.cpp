#include "Camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Window.hpp"
#include "Logger.hpp"

namespace engine
{
	void Camera::translate(float x, float y, float z)
	{
		translation += glm::vec3(x, y, z);
	}

	void Camera::rotate(float angle, float x, float y, float z)
	{
		rotation += glm::vec3(angle * x, angle * y, angle * z);
	}

	glm::mat4 Camera::getTransform()
	{
		glm::mat4 lookAtMatrix = glm::lookAt(glm::vec3(0, 0, 0), direction, glm::vec3(0, 1, 0));

		return lookAtMatrix;
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

		return glm::translate(glm::mat4(1.0f), translation) * rotationMat;
	}

	glm::mat4 Camera::getViewMatrix()
	{
		glm::vec3 worldUp = glm::vec3(0, 1, 0);
		glm::vec3 cameraRight = glm::normalize(glm::cross(direction, worldUp));
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, direction));

		glm::mat3 cameraBaseVectorsWorldSpace(cameraRight, cameraUp, -direction);
		glm::mat4 cameraRotation = glm::mat4(transpose(cameraBaseVectorsWorldSpace));
		glm::mat4 viewMatrix = cameraRotation * glm::translate(-translation);
		return viewMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		int width;
		int height;
		Window::getInstance().getWindowSize(&width, &height);

		float aspectRatio = float(width) / float(height);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		return projectionMatrix;
	}
}
