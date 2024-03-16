#include "ComponentFactory.hpp"
#include "Components/Component.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"
#include "Components/PointLightComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MultiplayerComponent.hpp"
#include "Components/ScriptableComponent.hpp"
#include "Components/ControllableComponent.hpp"
#include "Components/SpotLightComponent.hpp"
#include "Core/Logger.hpp"
#include "Core/ResourceManager.hpp"

namespace engine
{
	std::shared_ptr<Component> ComponentFactory::createComponent(std::string componentName)
	{
		LOG_TRACE("Creating component: {0}", componentName);
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
		else if (componentName == "SpotLight")
		{
			return std::make_shared<SpotLightComponent>();
		}
		else if (componentName == "Camera")
		{
			return std::make_shared<CameraComponent>();
		}
		else if (componentName == "Multiplayer")
		{
			return std::make_shared<MultiplayerComponent>();
		}
		else if (componentName == "Controllable")
		{
			return std::make_shared<ControllableComponent>();
		}
		else if (componentName == "Scriptable")
		{
			return std::make_shared<ScriptableComponent>();
		}

		LOG_INFO("Did not find component: {0} in list of default components, looking for script component");


		// Add .cs extension to the component if it is not already there
		if (componentName.find(".cs") == std::string::npos)
		{
			componentName += ".cs";
		}

		std::vector<std::string> allScriptComponents = ResourceManager::getInstance()->getAllCSharpScriptsInActiveGame();
		if (std::find(allScriptComponents.begin(), allScriptComponents.end(), componentName) == allScriptComponents.end())
		{
			LOG_ERROR("Could not find script: {0}", componentName);
			return nullptr;
		}

		std::shared_ptr<ScriptableComponent> scriptComponent = std::make_shared<ScriptableComponent>();
		scriptComponent->setScriptFileName(componentName);
		return scriptComponent;
	}
}