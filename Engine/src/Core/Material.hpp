#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Texture.hpp"
#include "Serializer/Serializable.hpp"
#include "Selectable.hpp"

namespace engine
{
	class Material : public Selectable, public Serializable
	{
	public:
		std::string getName() override
		{
			return name;
		}

		UUID getUUID() override
		{
			return uuid;
		}

		void setBaseColor(glm::vec3 color)
		{
			baseColor = color;
		}

		// Note: handeled separately, do not add to serializable variables
		bool isExternalMultiplayerObject = false;

		glm::vec3 baseColor{ 1,1,1 };
		float transparency = 1;
		glm::vec3 emission = { 0,0,0 };
		float roughness = 0;
		float shininess = 0.5;

		std::string name = "Default Material";

		std::weak_ptr<Texture> diffuseTexture;
		std::weak_ptr<Texture> specularTexture;

		UUID uuid{};

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			std::string* diffuseId = diffuseTexture.expired() ? &dummyId : &diffuseTexture.lock()->uuid.id;
			std::string* specularId = specularTexture.expired() ? &dummyId : &specularTexture.lock()->uuid.id;
			return
			{
				{ SerializableType::COLOR, "Base Color", "", &baseColor},
				{ SerializableType::FLOAT, "Transparency", "", &transparency},
				{ SerializableType::VECTOR3, "Emission", "", &emission},
				{ SerializableType::FLOAT, "Roughness", "", &roughness},
				{ SerializableType::FLOAT, "Shininess", "", &shininess},
				{ SerializableType::STRING, "Name", "", &name},
				{ SerializableType::STRING, "Diffuse Texture", "", diffuseId},
				{ SerializableType::STRING, "Specular Texture", "", specularId},
				{ SerializableType::STRING, "Id", "", &uuid}
			};
		};
	private:
		std::string dummyId = "";
	};
}