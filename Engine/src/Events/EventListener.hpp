#pragma once
#include <string>

namespace engine
{
	enum class EventType
	{
		QuitProgram,
		LAST
	};
	class EventListener
	{
	public:
		virtual void onEvent(EventType type, std::string message) = 0;
		virtual ~EventListener() = default;
	};
}