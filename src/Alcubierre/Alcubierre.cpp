#pragma once
#define GLFW_INCLUDE_NONE
#include <Alcubierre/Alcubierre.h>
#include <Alcubierre/Engine.h>
#include <Alcubierre/Debug/Log.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif // WIN32



int Alcubierre::argc = NULL;
char* Alcubierre::argv = NULL;
bool Alcubierre::Debug_Mode = true;

void Alcubierre::Initialize_Core()
{
	spdlog::set_level(spdlog::level::debug);
	try
	{
		auto file_logger = std::make_shared<spdlog::sinks::basic_file_sink_mt>("jatapt-log.txt", false);
#ifdef WIN32
			auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
			spdlog::default_logger()->sinks().push_back(msvc_sink);
#endif // WIN32

		spdlog::default_logger()->sinks().push_back(file_logger);
		spdlog::flush_on(spdlog::level::info);
		spdlog::debug("-------------------- Beginning of Log --------------------");
		//spdlog::set_default_logger();
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log init failed: " << ex.what() << std::endl;
	}
	Alcubierre::Debug::Log::Msg("Alcubierre","(%s %s) [%s %s] %s", PROJECT_NAME_READABLE, PROJECT_OS, PROJECT_BUILD_TYPE, PROJECT_VER, PROJECT_VER_TYPE, PROJECT_BUILD_DATE);
	Info = _Info();
	Basic_Config = _Basic_Config();
}

void Alcubierre::Accept_Arguments(int argc, char* argv)
{
	Alcubierre::argc = argc;
	Alcubierre::argv = argv;



}

void Alcubierre::Exit(int code)
{
	Alcubierre::Debug::Log::Msg("Alcubierre", "Got Exit Code (%i)", code);
	Alcubierre::Debug::Log::Msg("ERROR","Exiting...");
	exit(code);
}
