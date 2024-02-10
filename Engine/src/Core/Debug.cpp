#include "Debug.hpp"
#include <iostream>

namespace engine
{
	void Debug::Log(const std::string& message)
	{
		LogEntry log = LogEntry(message);
		LogQueue.push_back(log);

		std::cout << message << std::endl;
	}
}

