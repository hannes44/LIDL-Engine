#pragma once
#include "Component.hpp"
#include <glm/glm.hpp>

namespace engine
{
	class PointLightComponent : public Component
	{
	public:
		glm::vec3 color{ 1, 1, 1 };

		float intensity = 10;

		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;

		std::string getName() override { return name; };

		inline static const std::string name = "PointLight";

		std::shared_ptr<Component> clone() override {
			return std::make_shared<PointLightComponent>(*this);
		}

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::COLOR, "Color", "The color or the pointlight", &color},
				{SerializableType::FLOAT, "Intensity", "The intensity of the pointlight", &intensity},
				{SerializableType::FLOAT, "Constant", "The constant value of the pointlight", &constant},
				{SerializableType::FLOAT, "Linear", "The linear value of the pointlight", &linear},
				{SerializableType::FLOAT, "Quadratic", "The quadratic value of the pointlight", &quadratic},
			};
		};
	};
}