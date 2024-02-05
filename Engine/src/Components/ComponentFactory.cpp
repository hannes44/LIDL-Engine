#include "ComponentFactory.hpp"
#include "Components/Component.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"
#include "Components/PointLightComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Core/Logger.hpp"


namespace engine
{
	Component* ComponentFactory::createComponent(std::string componentName)
	{
		LOG_INFO("Creating component: {0}", componentName);
		if (componentName == "Mesh")
		{
			return new MeshComponent();
		}
		if (componentName == "Physics")
		{
			return new PhysicsComponent();
		}
		else if (componentName == "Box Collider")
		{
			return new BoxColliderComponent();
		}
		else if (componentName == "Sphere Collider")
		{
			return new SphereColliderComponent();
		}
		else if (componentName == "PointLight")
		{
			return new PointLightComponent();
		}
		else if (componentName == "Camera")
		{
			return new CameraComponent();
		}

		LOG_INFO("Did not find component: {0} in list of default components, looking for component dll");
		// TODO: Load component from dll

		LOG_WARN("Component: {0} not found", componentName);
		return nullptr;
	}
}