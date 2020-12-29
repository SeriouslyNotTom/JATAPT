#include <Alcubierre/IO/Network/Network_Core.h>
#include <Alcubierre/Alcubierre.h>
#include <Alcubierre/Debug/Log.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingsockets.h>
#include <stdlib.h>
#include <asio.hpp>

using namespace asio;
typedef asio::ip::tcp protocol;
typedef protocol::resolver resolver;

static void DebugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
{
	Alcubierre::Debug::Log::Msg("GameNetworkingSockets", "%s", pszMsg);
	fflush(stdout);
	if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
	{
		fflush(stdout);
		fflush(stderr);
		Alcubierre::Exit(-1);
	}
}

void Alcubierre::Net::init()
{
	SteamDatagramErrMsg errMsg;
	if (!GameNetworkingSockets_Init(nullptr, errMsg))
	{
		Alcubierre::Debug::Log::Msg("ERROR", errMsg);
	}
	SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);

}

void handle_resolve_query(const error_code& ec, resolver::iterator iter) {
	if (ec) return;

	resolver::iterator end;
	for (; iter != end; ++iter) {
		protocol::endpoint endpoint = *iter;
		std::cout << endpoint << std::endl;
	}
}


char* Alcubierre::Net::resolve(char* name)
{
	io_service io;
	resolver resolver(io);
	resolver::query query(name, "http");
	resolver::iterator iter = resolver.resolve(query);
	resolver::iterator end;
	std::vector<std::string> ends = std::vector<std::string>();
	while (iter != end)
	{
		ip::tcp::endpoint endpoint = *iter++;
		ends.push_back(endpoint.address().to_string());
	}
	const char* addr = ends[0].c_str();
	char* finaladdr = new char[255];
	memset(finaladdr, 0x00, 255);
	strcpy(finaladdr, addr);
	return finaladdr;
}
