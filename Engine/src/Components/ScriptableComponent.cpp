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

}
