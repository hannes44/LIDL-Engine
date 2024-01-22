#include "logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace engine
{
	void Logger::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		coreLogger = spdlog::stdout_color_mt("CORE");
		coreLogger->set_level(spdlog::level::trace);
		LOG_INFO("Logger initialized");
	}

	std::shared_ptr<spdlog::logger> Logger::coreLogger;
}



