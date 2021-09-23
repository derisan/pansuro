#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Log
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};


#define MK_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define MK_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define MK_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define MK_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define MK_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)
