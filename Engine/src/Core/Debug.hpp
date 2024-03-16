#pragma once
#include <string>
#include <vector>

namespace engine
{
	struct LogEntry
	{
		std::string message;
	};

	class Debug
	{
	public:
		static void Log(const std::string& message);
		inline static std::vector<LogEntry> LogQueue{};
	};

}