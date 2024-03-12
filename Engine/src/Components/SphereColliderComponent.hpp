#pragma once

#include "ColliderComponent.hpp"
#include "Core/BoundingBox.hpp"
#include "CameraComponent.hpp"

namespace engine
{

	class SphereColliderComponent : public ColliderComponent
	{
	public:
		float radius = 1;
		void drawCollider(CameraComponent* camera);
		BoundingBox getBoundingBox();
		SphereColliderComponent(glm::vec3 offset = glm::vec3(0, 0, 0), float radius = 1, bool isPlatform = false) : radius(radius), ColliderComponent(offset, glm::vec3(radius, radius, radius), isPlatform) {}

		std::string getName() override;

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return {
				{SerializableType::FLOAT, "Radius", "The radius of the collider", &radius},
				{SerializableType::VECTOR3, "Offset", "The offset of the collider", &offset},
				{SerializableType::VECTOR3, "Extent", "The extent of the collider", &extent} 
			};
		};

		std::shared_ptr<Component> clone() override
		{
			return std::make_shared<SphereColliderComponent>(*this);
		}
	};
}