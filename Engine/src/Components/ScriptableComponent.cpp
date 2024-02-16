#include "ScriptableComponent.hpp"
#include "ScriptEngine/ScriptEngine.hpp"

namespace engine
{
	ScriptableComponent::ScriptableComponent()
	{
		EventManager::getInstance().subscribe(EventType::ScriptsRecompiled, this);
	}
	void ScriptableComponent::update()
	{
		ScriptEngine::getInstance()->updateScriptableComponent(this);
	}

	void ScriptableComponent::initialize()
	{
		ScriptEngine::getInstance()->initializeScriptableComponent(this);
	}

	std::string ScriptableComponent::getScriptClassName()
	{
		std::string fileNameNoExtension = scriptFileName.substr(0, scriptFileName.find_last_of("."));
		return fileNameNoExtension;
	}

	void ScriptableComponent::onEvent(EventType type, std::string message)
	{
		// If the scripts are recompiled, we need to reinitialize the state
		if (type == EventType::ScriptsRecompiled)
		{
			stateIsInitialized = false;
		}
	}

}
