#pragma once
#include "EventListener.hpp"
#include <unordered_map>

namespace engine
{
	class EventManager
	{
	public:
		static EventManager& getInstance();
		~EventManager() = default;
		EventManager(const EventManager&) = delete;

		void subscribe(EventType type, EventListener& eventListener);
		void unsubscribe(EventType type, EventListener& eventListener);
		void notify(EventType type, std::string message);

	private:
		std::unordered_map<int, std::vector<EventListener*>> eventListeners{};
		EventManager();
		inline static EventManager* instance = nullptr;
	};
}