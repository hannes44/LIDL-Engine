#pragma once
#include <string>
#include <unordered_map>
#include <list>
#include "InputEvent.hpp"

/*
* This class is a singleton that maps actions to sets of keys
* To add an action, use addAction with the action name and a list of keys
* To remove an action, use removeAction with the action name and a list of keys
* Example: 	inputFramework.addAction("TEST", {Key::W, Key::SPACE});
* 			inputFramework.removeAction("TEST", {Key::W, Key::SPACE});
* _______________________________________________________________________
* When an action is fullfilled, the input framework will dispatch an event
* with the inputEventType set to "Action" and the name of the action
*/


namespace engine {
	class ActionMap
	{
	public:
		ActionMap(const ActionMap&) = delete;
		ActionMap& operator=(const ActionMap&) = delete;
		~ActionMap() = default;

		static ActionMap& getInstance();

		void addAction(const std::string& action, std::list<Key> keys);

		void removeAction(const std::string& action, std::list<Key> keys);

		std::list<Key> getKeys(const std::string& action);

		std::list<std::string> getActions();

	protected:
		ActionMap();
	private:
		inline static ActionMap* instance = nullptr;
		std::unordered_map<std::string, std::list<Key>> actionMap;
	};
}