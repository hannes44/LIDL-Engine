#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace engine
{
	class Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
	};

//#define ENABLE_TRACE_LOGGING

	// Core log macros
#if defined(_DEBUG) && defined(ENABLE_TRACE_LOGGING)
#define LOG_TRACE(...) Logger::getCoreLogger()->trace("Trace: " __VA_ARGS__ ) // Only log trace in debug mode
#else
#define LOG_TRACE(...)
#endif

#define LOG_INFO(...)  Logger::getCoreLogger()->info("Info: " __VA_ARGS__ )
#define LOG_WARN(...)  Logger::getCoreLogger()->warn("Warning: " __VA_ARGS__ )
#define LOG_ERROR(...) Logger::getCoreLogger()->error("Error: " __VA_ARGS__ )
#define LOG_FATAL(...) Logger::getCoreLogger()->critical("Fatal: " __VA_ARGS__ )
}