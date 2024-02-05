#include "Components/CameraComponent.hpp"
#include "Input/InputSystem.hpp"
#include "Core/Logger.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "Input/InputFramework.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Core/Window.hpp"
#include <iostream>
#include "Core/Bootstrap.hpp"
#include <imgui_impl_sdl3.h>

namespace engine
{
	CameraComponent::CameraComponent()
	{

	}

	void CameraComponent::update() {
	}

	void CameraComponent::translate(float x, float y, float z)
	{
		translation += glm::vec3(x, y, z);
	}

	void CameraComponent::rotate(float angle, float x, float y, float z)
	{
		rotation += glm::vec3(angle * x, angle * y, angle * z);
	}

	glm::mat4 CameraComponent::getTransform()
	{
		glm::mat4 lookAtMatrix = glm::lookAt(glm::vec3(0, 0, 0), direction, glm::vec3(0, 1, 0));

		return lookAtMatrix;
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

		return glm::translate(glm::mat4(1.0f), translation) * rotationMat;
	}

	glm::mat4 CameraComponent::getViewMatrix()
	{
		glm::vec3 worldUp = glm::vec3(0, 1, 0);
		glm::vec3 cameraRight = glm::normalize(glm::cross(direction, worldUp));
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, direction));

		glm::mat3 cameraBaseVectorsWorldSpace(cameraRight, cameraUp, -direction);
		glm::mat4 cameraRotation = glm::mat4(transpose(cameraBaseVectorsWorldSpace));
		glm::mat4 viewMatrix = cameraRotation * glm::translate(-translation);
		return viewMatrix;
	}

	glm::mat4 CameraComponent::getProjectionMatrix()
	{
		int width;
		int height;
		Window::getInstance().getWindowSize(&width, &height);

		float aspectRatio = float(width) / float(height);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		return projectionMatrix;
	}

	void CameraComponent::handleInput(const InputEvent& event) {

		InputEventType EventType = event.getEventType();

		// Handle key and mouse input here
		// If mouse button is pressed we want to control the camera
		if (EventType == InputEventType::MouseButtonDown && (Key)event.getButton() == Key::MOUSE_RIGHT && !(ImGui::GetIO().WantCaptureMouse)) {
			isMouseDragging = true;
		}
		// If mouse button is released we want to stop controlling the camera
		if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))) {
			isMouseDragging = false;
		}

		if ((EventType == InputEventType::MouseMotion || EventType == InputEventType::KeyDown || EventType == InputEventType::KeyHold)
			&& isMouseDragging && !(ImGui::GetIO().WantCaptureMouse)) {

			if (SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				glm::mat4 yaw = glm::rotate(rotationSpeed * -event.getX(), worldUp);
				glm::mat4 pitch = glm::rotate(rotationSpeed * -event.getY(), glm::normalize(glm::cross(direction, worldUp)));
				direction = glm::vec3(pitch * yaw * glm::vec4(direction, 0.0f));

				if (!ImGui::GetIO().WantCaptureKeyboard) {
					if (event.getKey() == Key::W) {
						translate(direction.x * movementSpeed, direction.y * movementSpeed, direction.z * movementSpeed);
					}

					else if (event.getKey() == Key::A) {
						glm::vec3 cameraRight = glm::normalize(glm::cross(direction, worldUp));
						translate(cameraRight.x * -movementSpeed, cameraRight.y * -movementSpeed, cameraRight.z * -movementSpeed);
					}

					else if (event.getKey() == Key::S) {
						translate(direction.x * -movementSpeed, direction.y * -movementSpeed, direction.z * -movementSpeed);
					}

					else if (event.getKey() == Key::D) {
						glm::vec3 cameraRight = glm::normalize(glm::cross(direction, worldUp));
						translate(cameraRight.x * movementSpeed, cameraRight.y * movementSpeed, cameraRight.z * movementSpeed);
					}

					else if (event.getKey() == Key::SPACE) {
						translate(worldUp.x * movementSpeed, worldUp.y * movementSpeed, worldUp.z * movementSpeed);
					}

					else if (event.getKey() == Key::LSHIFT) {
						translate(worldUp.x * -movementSpeed, worldUp.y * -movementSpeed, worldUp.z * -movementSpeed);
					}
				}
			}
		}
	}
}
