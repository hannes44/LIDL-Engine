#include "ScriptableComponent.hpp"
#include "ScriptEngine/ScriptEngine.hpp"

namespace engine
{
	ScriptableComponent::ScriptableComponent()
	{
		EventManager::getInstance().subscribe(EventType::ScriptsRecompiled, this);
		InputFramework::getInstance().addListener(this);
	}
	ScriptableComponent::~ScriptableComponent()
	{
		EventManager::getInstance().unsubscribe(EventType::ScriptsRecompiled, this);
		InputFramework::getInstance().removeListener(this);
	}
	void ScriptableComponent::update()
	{
		ScriptEngine::getInstance()->updateScriptableComponent(this);
	}

	void ScriptableComponent::initialize()
	{
		ScriptEngine::getInstance()->initializeScriptableComponent(this);
	}

	void ScriptableComponent::setScriptFileName(std::string scriptFileName)
	{
		this->scriptFileName = scriptFileName;
		ScriptEngine::getInstance()->initializeLuaStateForScriptableComponent(this);
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

	void ScriptableComponent::handleInput(const InputEvent& event)
	{
		if (!enableInput)
			InputFramework::getInstance().removeListener(this);

		ScriptEngine::getInstance()->handleInputForScriptableComponent(this, event);
	}

}
