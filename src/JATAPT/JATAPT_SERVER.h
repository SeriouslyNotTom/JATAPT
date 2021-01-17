#pragma once;

#include <JATAPT/JATAPT_COMMON.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <map>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingsockets.h>
#include <cereal/types/string.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/binary.hpp>

class Server_Config
{
public:
	const char* password;
	const char* listen_address;
	int listen_port;
	int blob_size;
	int blob_count;
	const char* xml_path;
	const char* spooled_path;
	const char* audio_path;
	const char* audio_web_prefix;
	int throttle_val;
};

enum ClientState		
{
	Connecting,
	Connected,
	Transferring
};

struct Client_t
{
	bool Authenticated = false;
	HSteamNetConnection my_conn;
	ClientState client_state;
	char* client_addr;
};

class blob
{
public:
	int raw_size;
	int rc_size;
	char* data;
	char* file_name;
	char* guid;
	bool lock = false;

	void init(int bs);
	void destroy();
};

struct Episode_Spool
{
	std::vector<JATAPT::COMMON::J_EP> spooled_episodes;
	int spooled_count = 0;
	time_t soonest_ep_date = 0;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(spooled_episodes, spooled_count, soonest_ep_date);
	}
};

struct Stored_Server_Data
{
	Episode_Spool Spool;
	JATAPT::COMMON::J_EP_SET Stored_Episodes;
	JATAPT::COMMON::J_FILE_SET Stored_Files;
};

class Server : private ISteamNetworkingSocketsCallbacks
{
private:
	//vars
	HSteamListenSocket ListenSock;
	ISteamNetworkingSockets* Interface;
	HSteamNetPollGroup PollGroup;
	
	std::map<HSteamNetConnection, Client_t> Client_List;
	std::vector<blob> blobs = std::vector<blob>();
	std::map<char*, int> blob_map = std::map<char*, int>();

	Stored_Server_Data Data;

	bool Add_To_Spool(JATAPT::COMMON::J_EP ep);
	bool Remove_From_Spool(JATAPT::COMMON::J_EP ep);

	void Save_Spooled_Episodes();
	void Load_Spooled_Episodes();
	void Check_Spooled_Episodes();

	void LoadEpisodes();
	void LoadFiles();

	bool Write_Episode_To_File(JATAPT::COMMON::J_EP ep, bool existing = false);

	void PollIncoming();
	virtual void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) override;
	void ProcessCommand(const char* pkt, HSteamNetConnection conn);
	bool CheckAuth(HSteamNetConnection conn, bool boot = false);
	void TryAuth(HSteamNetConnection conn);

public:
	void Run();
	void SendFailAuth(HSteamNetConnection conn);
};

namespace JATAPT
{
	namespace SERVER
	{
		void StartServer(char* config_path);
		void Tick();
	}
}