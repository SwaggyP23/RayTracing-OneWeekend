#pragma once

#include <iostream>

namespace rtow::Logger {

	struct Level
	{
		constexpr static const char* const Error = "\x1b[31m";
		constexpr static const char* const Info = "\x1b[32m";
		constexpr static const char* const Warning = "\x1b[33m";
		constexpr static const char* const Debug = "\x1b[34m";
		constexpr static const char* const Fine = "\x1b[97m";
	};

	struct Log
	{
		template<typename... Args>
		void operator()(const char* const level, Args&&... args) const
		{
			std::cout << level;
			((std::cout << std::forward<Args>(args) << ' '), ...);
			std::cout << Level::Fine;

			std::cout << std::endl;
		}
	};

}

#define RTOW_LOG_ERROR(...)      rtow::Logger::Log{}(rtow::Logger::Level::Error, __VA_ARGS__)
#define RTOW_LOG_WARNING(...)    rtow::Logger::Log{}(rtow::Logger::Level::Warning, __VA_ARGS__)
#define RTOW_LOG_INFO(...)       rtow::Logger::Log{}(rtow::Logger::Level::Info, __VA_ARGS__)
#define RTOW_LOG_DEBUG(...)      rtow::Logger::Log{}(rtow::Logger::Level::Debug, __VA_ARGS__)
#define RTOW_LOG(...)            rtow::Logger::Log{}(rtow::Logger::Level::Fine, __VA_ARGS__)