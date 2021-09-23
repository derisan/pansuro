#include "pch.h"
#include "Log.h"

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::OnInit()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_Logger = spdlog::stdout_color_mt("ProjectM");
	s_Logger->set_level(spdlog::level::trace);
}
