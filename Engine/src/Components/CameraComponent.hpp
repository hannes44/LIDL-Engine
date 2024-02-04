#pragma once
#include "Core/InputListener.hpp"
#include <glm/glm.hpp>
#include "Components/Component.hpp"

namespace engine
{
	// Could add the camera as a component to allow for multiple cameras and more control for the game creator
	class CameraComponent : public Component, public InputListener
	{
	public:
		CameraComponent();
		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);

		void handleInput(const InputEvent& event,const std::string& EventType) override;
		void update();

		bool isMainCamera = false;

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::FLOAT, "FOV", "Field of View of the camera", &fov},
				{SerializableType::FLOAT, "Near Plane", "Near plane of the camera", &nearPlane},
				{SerializableType::FLOAT, "Far Plane", "Far plane of the camera", &farPlane},
				{SerializableType::BOOLEAN, "Is Main Camera", "Is this the camera that will render the game", &isMainCamera},
			};
		};

		glm::mat4 getTransform();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		glm::vec3 translation {20, 20, 20};
		glm::vec3 rotation {90, 0, 0};
		glm::vec3 direction {-1, -1, -1};
		glm::vec3 worldUp {0.0f, 1.0f, 0.0f};

		
		float fov = 50;
		float nearPlane = 0.1;
		float farPlane = 1000;
		float rotationSpeed = 0.005f;
		float movementSpeed = 0.1f;	
		bool isMouseDragging;

	};
}