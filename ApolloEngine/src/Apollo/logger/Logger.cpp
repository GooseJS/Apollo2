#include "Apollo/logger/Logger.h"

namespace Apollo
{
	std::shared_ptr<spdlog::logger> Logger::_coreLogger;
	std::shared_ptr<spdlog::logger> Logger::_clientLogger;

	void Logger::init()
	{
		spdlog::set_pattern("%^[%T] [%l] %n: %v%$");
		_coreLogger = spdlog::stdout_color_mt("APOLLO");
		_coreLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("APP");
		_clientLogger->set_level(spdlog::level::trace);
	}
}