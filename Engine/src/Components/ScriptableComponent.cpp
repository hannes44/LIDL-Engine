#include "ScriptableComponent.hpp"
#include "ScriptEngine/ScriptEngine.hpp"

namespace engine
{
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

}
