#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Apollo/Core.h"
#include "Apollo/logger/Logger.h"

namespace Apollo
{
	class APOLLO_API Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return _coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
	};
}

#define AP_ENGINE_TRACE(...)	::Apollo::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define AP_ENGINE_INFO(...)		::Apollo::Logger::getCoreLogger()->info(__VA_ARGS__)
#define AP_ENGINE_WARN(...)		::Apollo::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define AP_ENGINE_ERROR(...)	::Apollo::Logger::getCoreLogger()->error(__VA_ARGS__)
#define AP_ENGINE_CRITICAL(...)	::Apollo::Logger::getCoreLogger()->critical(__VA_ARGS__)

#define AP_TRACE(...)			::Apollo::Logger::getClientLogger()->trace(__VA_ARGS__)
#define AP_INFO(...)			::Apollo::Logger::getClientLogger()->info(__VA_ARGS__)
#define AP_WARN(...)			::Apollo::Logger::getClientLogger()->warn(__VA_ARGS__)
#define AP_ERROR(...)			::Apollo::Logger::getClientLogger()->error(__VA_ARGS__)
#define AP_CRITICAL(...)		::Apollo::Logger::getClientLogger()->critical(__VA_ARGS__)