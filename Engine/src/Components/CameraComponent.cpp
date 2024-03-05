#include "Components/CameraComponent.hpp"
#include "Core/Logger.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "Input/InputFramework.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Core/Window.hpp"
#include <iostream>
#include <imgui_impl_sdl3.h>
#include "CameraComponent.hpp"

namespace engine
{
	void CameraComponent::update() 
	{
		
	}

	Transform CameraComponent::getTransform()
	{
		return gameObject->transform;
	}

	glm::mat4 CameraComponent::getViewMatrix()
	{
		Transform transform = getTransform();
		glm::mat4 viewMatrix = glm::inverse(transform.transformMatrix);

		return viewMatrix;
	}

	// Returns the projection matrix for the window size
	glm::mat4 CameraComponent::getProjectionMatrix()
	{
		int width;
		int height;
		Window::getInstance().getWindowSize(&width, &height);

		float aspectRatio = float(width) / float(height);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		return projectionMatrix;
	}

	// Returns the projection matrix for the given width and height
	glm::mat4 CameraComponent::getProjectionMatrix(int width, int height)
	{
		float aspectRatio = float(width) / float(height);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		return projectionMatrix;
	}
}
