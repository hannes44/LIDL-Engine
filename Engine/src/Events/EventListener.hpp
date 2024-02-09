#pragma once
#include <string>

namespace engine
{
	enum class EventType
	{
		QuitProgram,
		SelectableDeleted,
		SelectableAdded,
		Last
	};
	class EventListener
	{
	public:
		virtual void onEvent(EventType type, std::string message) = 0;
		virtual ~EventListener() = default;
	};
}