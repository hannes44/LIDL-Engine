#pragma once
#include "InputListener.hpp"
#include <glm/glm.hpp>

namespace engine
{
	// Could add the camera as a component to allow for multiple cameras and more control for the game creator
	class Camera : public InputListener
	{
	public:
		Camera();
		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);

		void handleInput(const InputEvent& event,const std::string& EventType) override;
		void update();

		glm::mat4 getTransform();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		glm::vec3 translation {20, 20, 20};
		glm::vec3 rotation {0, 0, 0};
		glm::vec3 direction {-1, -1, -1};
		glm::vec3 worldUp {0.0f, 1.0f, 0.0f};

		
		float fov = 50;
		float nearPlane = 0.1;
		float farPlane = 100;
		float cameraSpeed = 100; // Unused?
		float rotationSpeed = 0.005f;
		float movementSpeed = 0.1f;	
		bool isMouseDragging;
	};
}