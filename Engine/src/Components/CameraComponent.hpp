#pragma once
#include "Input/InputListener.hpp"
#include <glm/glm.hpp>
#include "Components/Component.hpp"

namespace engine
{
	// Could add the camera as a component to allow for multiple cameras and more control for the game creator
	class CameraComponent : public Component, public InputListener
	{
	public:
		CameraComponent() {};

		void update(float deltaTime) override;

		Transform getTransform();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getProjectionMatrix(int width, int height);
		std::string getName() override { return "Camera"; };

		std::shared_ptr<Component> clone() override { return std::make_shared<CameraComponent>(*this); }

		float fov = 50;
		float nearPlane = 0.1;
		float farPlane = 1000;
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
	};
}