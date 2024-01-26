#pragma once
#include "Component.hpp"
#include <glm/glm.hpp>

namespace engine
{
	class PointLightComponent : public Component
	{
	public:
		glm::vec3 color{1, 1, 1};

		float intensity = 10;

		std::string getName() override { return "PointLight"; };
	};
}