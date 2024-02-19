#include "ActionMap.hpp"

namespace engine {

	ActionMap& ActionMap::getInstance() {
		static ActionMap instance;
		return instance;
	}

	void ActionMap::addAction(const std::string& action, std::list<Key> keys) {
		actionMap[action] = keys;
	}

	void ActionMap::removeAction(const std::string& action, std::list<Key> keys) {
		actionMap.erase(action);
	}

	std::list<Key> ActionMap::getKeys(const std::string& action) {
		return actionMap[action];
	}

	std::list<std::string> ActionMap::getActions() {
		std::list<std::string> actions;
		for (auto it = actionMap.begin(); it != actionMap.end(); it++) {
			actions.push_back(it->first);
		}
		return actions;
	}

	ActionMap::ActionMap()
	{
	}

}