#include "ComponentFactory.hpp"
#include "Components/Component.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"
#include "Components/PointLightComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/ScriptableComponent.hpp"
#include "Core/Logger.hpp"


namespace engine
{
	std::shared_ptr<Component> ComponentFactory::createComponent(std::string componentName)
	{
		LOG_INFO("Creating component: {0}", componentName);
		if (componentName == "Mesh")
		{
			return std::make_shared<MeshComponent>();
		}
		else if (componentName == "Physics")
		{
			return std::make_shared<PhysicsComponent>();
		}
		else if (componentName == "Box Collider")
		{
			return std::make_shared<BoxColliderComponent>();
		}
		else if (componentName == "Sphere Collider")
		{
			return std::make_shared<SphereColliderComponent>();
		}
		else if (componentName == "PointLight")
		{
			return std::make_shared<PointLightComponent>();
		}
		else if (componentName == "Camera")
		{
			return std::make_shared<CameraComponent>();
		}
		else if (componentName == "Scriptable")
		{
			return std::make_shared<ScriptableComponent>();
		}

		LOG_INFO("Did not find component: {0} in list of default components, looking for component dll");
		// TODO: Load component from dll

		LOG_WARN("Component: {0} not found", componentName);
		return nullptr;
	}
}