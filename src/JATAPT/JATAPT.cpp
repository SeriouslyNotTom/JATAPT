#pragma once

#include <JATAPT/JATAPT.h>
#include <JATAPT/JATAPT_COMMON.h>
#include <Alcubierre/Libraries/Debug/ImGui_Handler.h>
#include <Alcubierre/Alcubierre.h>
#include <Alcubierre/Engine.h>
#include <Alcubierre/Debug/Tripwire/tripwire.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Alcubierre/Render/Primative.h>
#include <Alcubierre/Render/Text.h>
#define ASIO_NO_WIN32_LEAN_AND_MEAN
#include <Alcubierre/Engine.h>
#include <stdlib.h>
#include <imgui.h>
#include <string.h>
#include <sys/stat.h>
#include <algorithm>
#include <JATAPT/JATAPT_SERVER.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <Alcubierre/IO/Network/Network_Core.h>
#include <thread>
#include <mio/mmap.hpp>
#include <CRC.h>
#include <nlohmann/json.hpp>
#include <Alcubierre/IO/FIleUtils.h>
#include <JATAPT/JATAPT_CLIENT.h>
#include <ctime>
#include <tinyxml2.h>
#include <Alcubierre/Libraries/Utilities/RandomUtils.h>
#include <signal.h>
#include <JATAPT/Network/Packets.h>

using namespace Alcubierre::Engine::Window;
using json = nlohmann::json;
using namespace JATAPT::COMMON::NET;

int main(int argc, char* argv[])
{

	try
	{
		bool ServerMode = false;

		JATAPT::COMMON::init_common();

		if (argc > 0) {
			for (int i = 0; i < argc; i++)
			{
				if (std::string(argv[i]) == "-s" || std::string(argv[i]) == "-S")
				{
					ServerMode = true;
				}
			}
		}

		if (ServerMode == true)
		{

			char* config_file = NULL;

			//all of this is going to be converted into alcubierre's parameter/config system
			for (int i = 0; i < argc; i++)
			{
				if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--config")
				{
					if (i + 1 == argc) {
						Alcubierre::Debug::Log::Msg("ERROR", "you must supply a .json config file with the -c or --config parameter; like this: -c jatapt-server.json or --config jatapt-server.json");
						Alcubierre::Exit(-666);
						break;
					}
					config_file = argv[i + 1];
				}
			}

			if (config_file == NULL)
			{
				if (Alcubierre::File::Util::FileExists("jatapt-server.json"))
				{
					config_file = "jatapt-server.json";
				}
				else
				{
					Alcubierre::Debug::Log::Msg("ERROR", "No Config file supplied/found please specify one with command line argument --config <file> or -c <file>");
					Alcubierre::Exit(-1);
				}
			}

			JATAPT::SERVER::StartServer(config_file);
		}
		else {
			JATAPT::CLIENT::Init(argc, argv);
			return 0;
		}

	}
	catch (...)
	{
		Alcubierre::Debug::Log::Msg("TRIPWIRE", "Unhandled Exception Caught");
	}

	Alcubierre::Exit(0);
}

#ifdef WIN32

#include<shellapi.h>
#include <WinUser.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	AttachConsole(ATTACH_PARENT_PROCESS);
	int argc;
	LPWSTR* argv = CommandLineToArgvW((LPWSTR)lpCmdLine, &argc);
	SetProcessDPIAware();
	return main(argc, (char**)argv);
}

#endif // WIN32