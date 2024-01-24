#pragma once
#include <glm/glm.hpp>
#include <string>

namespace engine
{
	class Material
	{
	public:
		glm::vec3 baseColor{ 1,0,0 };

		float transparency = 1;
		glm::vec3 emission = { 0,0,0 };
		float roughness = 0;
		float shininess = 0.5;

		std::string name = "Default Material";
	};
}