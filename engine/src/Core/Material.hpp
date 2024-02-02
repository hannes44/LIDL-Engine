#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Texture.hpp"

namespace engine
{
	class Material
	{
	public:
		glm::vec3 baseColor{ 1,1,1 };

		float transparency = 1;
		glm::vec3 emission = { 0,0,0 };
		float roughness = 0;
		float shininess = 0.5;

		std::string name = "Default Material";

		std::weak_ptr<Texture> diffuseTexture;
		std::weak_ptr<Texture> specularTexture;
	};
}