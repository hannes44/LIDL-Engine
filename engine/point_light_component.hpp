#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

namespace engine
{
	class PointLightComponent : public Component
	{
	public:
		glm::vec3 color;

		float intensity;
	};
}