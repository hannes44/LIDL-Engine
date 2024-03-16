#include "EventManager.hpp"

namespace engine
{
	EventManager::EventManager()
	{
		for (int i = 0; i < (int)EventType::Last; i++)
		{
			eventListeners[i] = std::vector<EventListener*>();
		}
	}

	EventManager& EventManager::getInstance()
	{
		if (instance == nullptr)
			instance = new EventManager();

		return *instance;
	}

	void EventManager::subscribe(EventType type, EventListener* eventListener)
	{
		eventListeners[(int)type].push_back(eventListener);
	}

	void EventManager::unsubscribe(EventType type, EventListener* eventListener)
	{
		int i = 0;
		std::vector<EventListener*> listeners = eventListeners[(int)type];
		for (auto& listener : listeners)
		{
			if (listener == eventListener)
			{
				eventListeners[(int)type].erase(eventListeners[(int)type].begin() + i);
				break;
			}
			i++;
		}
	}

	void EventManager::notify(EventType type, std::string message)
	{
		std::vector<EventListener*> listeners = eventListeners[(int)type];
		for (auto& listener : listeners)
		{
			listener->onEvent(type, message);
		}
	}
}

