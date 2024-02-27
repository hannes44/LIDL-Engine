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
				{SerializableType::FLOAT, "Intensity", "The intensity of the pointlight", &intensity}
			};
		};
	};
}