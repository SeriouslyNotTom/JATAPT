#pragma once

#include <JATAPT/JATAPT_SERVER.h>
#include <JATAPT/JATAPT_COMMON.h>
#include <sys/stat.h>
#include <Alcubierre/Alcubierre.h>
#include <Alcubierre/IO/Network/Network_Core.h>

#include <Alcubierre/build_info.h>
#include <stdlib.h>
#include <iostream>
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <thread>
#include <nlohmann/json.hpp>
#include <mio/mmap.hpp>
#include <Alcubierre/IO/FIleUtils.h>
#include <Alcubierre/Libraries/Utilities/RandomUtils.h>
#include <exception>
#include <algorithm>
#include <alphanum.hpp>
#include <crossguid/guid.hpp>
#define MINIMP3_IMPLEMENTATION
#include <minimp3.h>
#include <fstream>
#include <ctime>
#include <signal.h>
#include <JATAPT/Network/Packets.h>

using namespace std;
using json = nlohmann::json;

using namespace JATAPT::COMMON;

Server_Config config;

void blob::init(int bs)
{
	data = new char[bs];
	raw_size = bs;
	memset(data, 0x00, raw_size);
}

void blob::destroy()
{
	memset(data, 0x00, raw_size);
	free(data);
	raw_size = 0;
	rc_size = 0;
}

bool Server::Add_To_Spool(JATAPT::COMMON::J_EP ep)
{
	time_t this_ep_pubtime = ep.episode_publication_time;

	JATAPTEpisodeVerifyState_ state = JATAPT::COMMON::Verify_Episode(ep);
	if (state & JATAPTEpisodeVerifyState_TitleDescriptionSubtitleSummaryFileNameFilePathPubDate)
	{
		Alcubierre::Debug::Log::Msg("ERROR", "Episode attempting to be spooled does not have Title and/or Description, Subtitle, File name, Publication Date, Summary");
		return false;
	}
	else {
		Data.Spool.spooled_episodes.push_back(ep);
		Data.Spool.spooled_count++;

		//check if soonest ep time is not defined and update if so
		if (Data.Spool.soonest_ep_date == 0) { Data.Spool.soonest_ep_date = this_ep_pubtime; }
		else {if (Data.Spool.soonest_ep_date > this_ep_pubtime) { Data.Spool.soonest_ep_date = this_ep_pubtime; }}
		
		return true;
	}
	return false;
}

bool Server::Remove_From_Spool(JATAPT::COMMON::J_EP ep)
{
	bool found = false;
	if (Data.Spool.spooled_count > 0)
	{
		std::vector<JATAPT::COMMON::J_EP> new_eps = std::vector<JATAPT::COMMON::J_EP>();
		for (int i = 0; i <= Data.Spool.spooled_episodes.size(); i++)
		{
			JATAPT::COMMON::J_EP check_ep = Data.Spool.spooled_episodes[i];
			if (ep.guid != check_ep.guid)
			{
				new_eps.push_back(check_ep);
			}
			else { found = true; }
		}
	}
	return found;
}

void Server::Save_Spooled_Episodes()
{

	if (Data.Spool.spooled_count > 0)
	{
		//for (int i = 0; i <= episode_spool.spooled_count; i++)
		//{
		//	//verify the episode i guess
		//}

		std::ofstream ofs(config.spooled_path, std::ios::binary);
		cereal::BinaryOutputArchive archive_output(ofs);

		archive_output(Data.Spool);
	}

	/*if (Spooled_Episodes.size() > 0)
	{
		json spooled;
		for (int i = 0; i < Spooled_Episodes.size(); i++)
		{
			J_EP ep = Spooled_Episodes[i];
			json ep_json;
			ep_json["title"] = ep.title;
			ep_json["description"] = ep.description;
			ep_json["summary"] = ep.summary;
			ep_json["subtitle"] = ep.subtitle;
			ep_json["audio_url"] = ep.audio_url;
			ep_json["publication_date"] = ep.publication_date;
			ep_json["guid"] = ep.guid;
			std::string dump = ep_json.dump();
			spooled[i] = dump.c_str();
		}

		std::string spooled_eps_json = spooled.dump();
		size_t len = spooled_eps_json.size();

		ofstream outfile;
		outfile.clear();
		outfile.open(config.spooled_path);

		for (int i = 0; i <= len; i++)
		{
			outfile << spooled_eps_json[i];
		}
		Alcubierre::Debug::Log::Msg("Jatapt", "Saved %i episodes to spool file", Spooled_Episodes.size());
		outfile.close();
	}
	else {
		ofstream outfile;
		outfile.clear();
		outfile.open(config.spooled_path);
		outfile << char(4);
		outfile.close();
	}*/
}

void Server::Load_Spooled_Episodes()
{

	if (Alcubierre::File::Util::FileExists(config.spooled_path))
	{
		std::ifstream is(config.spooled_path, std::ios::binary);
		cereal::BinaryInputArchive inarchive(is);

		try {
			inarchive(Data.Spool);
		}
		catch (cereal::Exception e)
		{

		}

	}
	

	/*std::vector<JATAPT::COMMON::J_EP> loaded_eps;
	bool exists = Alcubierre::File::Util::FileExists(config.spooled_path);
	if (exists)
	{
		char* file = Alcubierre::File::Util::Load_File_memmap(config.spooled_path);
		if (strlen(file) <= 1) { return; }
		try {
			json json_eps = json::parse(file);

			for (auto &it :json_eps)
			{
				JATAPT::COMMON::J_EP ep = JATAPT::COMMON::Deserialize_Episode(it);
				JATAPTEpisodeVerifyState_ verif_state = JATAPT::COMMON::Verify_Episode(ep);
				if (verif_state & TitleDescriptionSubtitleFilePubDateSummary)
				{
					Alcubierre::Debug::Log::Msg("JATAPT", "Episode loaded from spool file was missing data");
				}
				else {
					loaded_eps.push_back(ep);
				}
			}

			for (int i = 0; i < loaded_eps.size(); i++)
			{
				Server::Spooled_Episodes.push_back(loaded_eps[i]);
				struct tm ep_date;
				ParseRFC822(loaded_eps[i].publication_date.c_str(), &ep_date);
				const time_t t_now = std::time(0);
				struct tm* time_now = localtime(&t_now);
				const time_t t_episode = tm_to_time_t(&ep_date);
				string t_string = "";

				if (t_episode > t_now)
				{
					if ((ep_date.tm_yday - time_now->tm_yday) > 0) { t_string += to_string(ep_date.tm_yday - time_now->tm_yday) + std::string(" Days"); }
					else {
						bool hours = false;
						if ((t_episode - t_now) >= (60 * 60)) {
							t_string += to_string((int)ceil((t_episode - t_now) / (60 * 60))) + std::string(" Hours"); hours = true;
							int hour_calc = ceil((t_episode - t_now) / (60 * 60));
							t_string += " And ";
							t_string += to_string((int)ceil(((t_episode - t_now) - (hour_calc * 60 * 60)) / (60))) + std::string(" Minutes");
						}
						if ((t_episode - t_now) <= (60 * 60)) { if (hours == true) { t_string += " And "; } t_string += to_string((int)ceil((t_episode - t_now) / (60))) + std::string(" Minutes"); }
					}
				}
				else {
					t_string = "Immediately";
				}

				Alcubierre::Debug::Log::Msg("JATAPT", "Episode : [%s] set to publish in %s", loaded_eps[i].title.c_str(), t_string.c_str());
			}

			if (loaded_eps.size() > 0)
			{
				Alcubierre::Debug::Log::Msg("Jatapt", "Loaded %i Spooled Episodes", loaded_eps.size());
			}

		}
		catch (json::parse_error e)
		{
			Alcubierre::Debug::Log::Msg("ERROR", "Could not parse spooled episode JSON \r\n%s",e.what());
		}
	}*/
}

void Server::Check_Spooled_Episodes()
{

	if (Data.Spool.spooled_count > 0)
	{
		time_t t_now = std::time(0);
		
		if (Data.Spool.soonest_ep_date <= t_now)
		{
			Episode_Spool new_spool;
			for (int i = 0; i <= Data.Spool.spooled_episodes.size(); i++)
			{
				J_EP this_ep = Data.Spool.spooled_episodes[i];
				time_t this_ep_time = this_ep.episode_publication_time;
				
				if (t_now >= this_ep_time)
				{
					Server::Write_Episode_To_File(this_ep, false);
				}
				else {
					new_spool.spooled_episodes.push_back(this_ep);
					new_spool.spooled_count++;
					if (new_spool.soonest_ep_date == 0 | this_ep_time < new_spool.soonest_ep_date){new_spool.soonest_ep_date = this_ep_time;}
				}

			}
			Data.Spool = new_spool;
		}
	}

	//if (Spooled_Episodes.size() <= 0) { return; }
	//std::time_t t = std::time(0);   // get time now
	//tm* td = localtime(&t);
	//bool didwork = false;

	//std::vector<JATAPT::COMMON::J_EP> newvec;
	//for(int i = 0; i<Spooled_Episodes.size(); i++)
	//{
	//	JATAPT::COMMON::J_EP ep = Spooled_Episodes[i];
	//	struct tm ep_time;
	//	ParseRFC822(ep.publication_date.c_str(),&ep_time);
	//	std::time_t ti = tm_to_time_t(&ep_time);

	//	//Alcubierre::Debug::Log::Msg("Time-Test","time_now: %i : ep_time: %i",t,ti);
	//	if (ti <= t) {
	//		bool existing = (ep.guid != "" | ep.guid.size() > 0);
	//		Alcubierre::Debug::Log::Msg("JATAPT", "Publishing Episode: [%s] to XML", ep.title.c_str());
	//		Write_Episode_To_File(ep, existing);
	//		didwork = true;
	//	}
	//	else {
	//		newvec.push_back(Spooled_Episodes[i]);
	//	}
	//}
	//if (didwork)
	//{
	//	Spooled_Episodes = newvec;
	//	Save_Spooled_Episodes();
	//}

}

void Server::Run()
{
	//todo
	Alcubierre::Debug::Log::Msg("Jatapt", "Loading Files");
	//LoadFiles();
	//Alcubierre::Debug::Log::Msg("Jatapt", "Loaded %i Files", Files.size());
	Alcubierre::Debug::Log::Msg("Jatapt", "Loading Episodes");
	LoadEpisodes();
	//Alcubierre::Debug::Log::Msg("Jatapt", "Loaded %i Episodes", Server::Live_Episodes.size());

	/*Alcubierre::Debug::Log::Msg("Jatapt", "Allocating blobs");
	try {
		for (int i = 0; i < config.blob_count; i++)
		{
			blob b = blob();
			b.init(config.blob_size);
			blobs.push_back(b);
			Alcubierre::Debug::Log::Msg("Jatapt", "blob %i allocated with size: %i MB", i, (int)(config.blob_size / 1024 / 1024));
		}
	}
	catch (std::bad_alloc& b)
	{
		Alcubierre::Debug::Log::Msg("ERROR", "COULD NOT ALLOCATE MEMORY BLOBS FOR FILES %s",b.what());
		Alcubierre::Exit(-2);
	}*/

	Alcubierre::Debug::Log::Msg("Jatapt", "Loading spooled episodes");
	Load_Spooled_Episodes();

	Interface = SteamNetworkingSockets();
	SteamNetworkingIPAddr serverLocalAddr;
	serverLocalAddr.Clear();
	serverLocalAddr.ParseString(config.listen_address);
	serverLocalAddr.m_port = config.listen_port;
	ListenSock = Interface->CreateListenSocketIP(serverLocalAddr, 0, nullptr);
	Interface->InitAuthentication();
	if (ListenSock == k_HSteamListenSocket_Invalid)
	{
		Alcubierre::Debug::Log::Msg("ERROR", "Could Not Start server on port %i, is there some program/service already running on that port?",config.listen_port);
		Alcubierre::Exit(-666);
	}
	char serverStr[SteamNetworkingIPAddr::k_cchMaxString];
	serverLocalAddr.ToString(serverStr, sizeof(serverStr), true);
	Alcubierre::Debug::Log::Msg("Jatapt","max packet size: %iKB", k_cbMaxSteamNetworkingSocketsMessageSizeSend/1024);
	Alcubierre::Debug::Log::Msg("Jatapt", "Listening on %s", serverStr);
	Client_List = std::map<HSteamNetConnection, Client_t>();
	Client_List.clear();

	PollGroup = Interface->CreatePollGroup();
	if (PollGroup == k_HSteamNetPollGroup_Invalid)
	{
		Alcubierre::Debug::Log::Msg("ERROR", "Could not create poll group");
		Alcubierre::Exit(-666);
	}

	while (true)
	{
		PollIncoming();
		Interface->RunCallbacks(this);
		Check_Spooled_Episodes();
		std::this_thread::sleep_for(std::chrono::milliseconds(config.throttle_val));
	}

}

void Server::PollIncoming()
{
	ISteamNetworkingMessage* incoming = nullptr;
	int numMsg = Interface->ReceiveMessagesOnPollGroup(PollGroup, &incoming, 1);
	if (numMsg == 0) { return; }
	if (numMsg > 0)
	{
		Alcubierre::Debug::Log::Msg("Network", "GOT MSG");
		//fprintf(stdout, "%s\r\n", incoming->m_pData);
		if (incoming->m_cbSize>0)
		{
			std::string incoming_data;
			incoming_data.assign((const char*)incoming->m_pData, incoming->m_cbSize);
			const char* data = incoming_data.c_str();
			ProcessCommand(data, incoming->m_conn);
		}
	}

}

bool Server::CheckAuth(HSteamNetConnection conn, bool boot)
{
	if (Client_List[conn].Authenticated != true)
	{
		json pkt;
		pkt[J_PF_HEADER] = J_CH::AUTH_FAIL;
		std::string dmp = pkt.dump();
		Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
		if (boot) { Interface->CloseConnection(conn, -666, "Not Authenticated", false); }
		return false;
	}
	//making god dang sure that the client IS authenticated or atleast they somehow managed to set their own authenticated flag to true
	if (Client_List[conn].Authenticated == true)
	{
		return true;
	}
	//just incase or more accurately... edgecase
	//that was a good joke and you laughed at it.
	if (boot) { Interface->CloseConnection(conn, -666, "Not Authenticated", false); }
	return false;
}

void Server::TryAuth(HSteamNetConnection conn)
{
	//never check if the client is authenticated already, since if someone managed to exploit auth status they will have it permanently. even through auth checks
	Client_List[conn].Authenticated = false;
	json pkt;
	pkt[J_PF_HEADER] = J_CH::AUTH_CHECK;
	std::string dmp = pkt.dump();
	Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Server::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	char temp[1024];

	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_None:
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
		{
			Client_List.erase(pInfo->m_hConn);
		}
		Interface->CloseConnection(pInfo->m_hConn, 0, nullptr, 0);
		Alcubierre::Debug::Log::Msg("Network", "Client: %s Disconnected, Reason: %s", pInfo->m_info.m_szConnectionDescription, pInfo->m_info.m_szEndDebug);
		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
	{
		Alcubierre::Debug::Log::Msg("Network", "New connection request from %s %s", pInfo->m_info.m_szConnectionDescription, pInfo->m_info.m_szEndDebug);
		if (Interface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
		{
			Interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
			Alcubierre::Debug::Log::Msg("Error", "Cant accept connection from %s", pInfo->m_info.m_szConnectionDescription);
		}
		Alcubierre::Debug::Log::Msg("Network", "Client %s Connected Successfully", pInfo->m_info.m_szConnectionDescription);
		Client_List[pInfo->m_hConn];
		Client_t* cl = &Client_List[pInfo->m_hConn];
		cl->client_state = ClientState::Connected;
		cl->my_conn = pInfo->m_hConn;
		char* addr = new char[16];
		SteamNetworkingIPAddr_ToString(&pInfo->m_info.m_addrRemote, addr, sizeof(addr), false);
		if (addr == "::") { addr == "localhost"; }
		cl->client_addr = addr;
		Interface->SetConnectionPollGroup(pInfo->m_hConn, PollGroup);

		TryAuth(pInfo->m_hConn);

		break;
	}
	}

}

void Server::ProcessCommand(const char* pkt, HSteamNetConnection conn)
{
	//todo replace all this

	//json data;
	//try{ data = json::parse(pkt); }
	//catch (nlohmann::json::exception e)
	//{
	//	Alcubierre::Debug::Log::Msg("ERROR", "INCOMING PACKET IS NOT VALID JSON OR COULD NOT BE PARSED");
	//	//fprintf(stdout, "reason: %s\r\n", e.what);
	//	return;
	//}
	//Alcubierre::Debug::Log::Msg("Network", "Packet Data: %s", pkt);
	//J_CH header = J_CH(data[J_PF_HEADER]);
	//switch (header)
	//{

	//	case J_CH::AUTH_RESPONSE:
	//	{
	//		if (strlen(config.password) > 0)
	//		{
	//			if (data[J_PF_PASSWORD] == std::string(config.password))
	//			{
	//				Client_List[conn].Authenticated = true;
	//				json pkt;
	//				pkt[J_PF_HEADER] = J_CH::AUTH_SUCCESS;
	//				std::string dmp = pkt.dump();
	//				Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
	//				break;
	//			}

	//			json pkt;
	//			pkt[J_PF_HEADER] = J_CH::AUTH_FAIL;
	//			std::string dmp = pkt.dump();
	//			Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
	//			break;
	//		}
	//		else {
	//			Client_List[conn].Authenticated = true;
	//			json pkt;
	//			pkt[J_PF_HEADER] = J_CH::AUTH_SUCCESS;
	//			std::string dmp = pkt.dump();
	//			Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
	//			break;
	//		}
	//	}

	//	case J_CH::EPISODE_QUERY:
	//	{
	//		if (CheckAuth(conn) != true) { break; }
	//		if (data[J_PF_QUERY] == "FULLSET")
	//		{
	//			Server::LoadEpisodes();
	//			json pkt;
	//			pkt[J_PF_HEADER] = J_CH::EPISODE_QUERY;
	//			pkt[J_PF_QUERY] = "FULLSET RESPONSE";
	//			pkt[J_PF_DATA] = Episode_JSON_Data;
	//			std::string dmp = pkt.dump();
	//			Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
	//			break;
	//		}
	//	}

	//	case J_CH::FILES_QUERY:
	//	{
	//		if (CheckAuth(conn) != true) {break; }
	//		if (data[J_PF_QUERY] == "FULLSET")
	//		{
	//			Server::LoadFiles();
	//			json pkt;
	//			pkt[J_PF_HEADER] = J_CH::FILES_QUERY;
	//			pkt[J_PF_QUERY] = "FULLSET RESPONSE";
	//			pkt[J_PF_DATA] = File_JSON_Data;
	//			std::string dmp = pkt.dump();
	//			Interface->SendMessageToConnection(conn, dmp.c_str(), (uint32)dmp.length(), k_nSteamNetworkingSend_Reliable, nullptr);
	//			break;
	//		}
	//	}

	//	case J_CH::EPISODE_EDIT:
	//	{
	//		if (CheckAuth(conn) != true) { break; }
	//		J_FC Operation = (data[J_PF_OPERATION]);
	//		switch (Operation)
	//		{
	//			case J_FC::EPISODE_NEW:
	//			{
	//				JATAPT::COMMON::J_EP ep = JATAPT::COMMON::Deserialize_Episode(data[J_PF_DATA]);
	//				struct tm ep_date;
	//				ParseRFC822(ep.publication_date.c_str(), &ep_date);
	//				const time_t t_now = std::time(0);
	//				struct tm* time_now = localtime(&t_now);
	//				const time_t t_episode = tm_to_time_t(&ep_date);
	//				string t_string = "";

	//				if (t_episode > t_now)
	//				{
	//					if ((ep_date.tm_yday - time_now->tm_yday) > 0) { t_string += to_string(ep_date.tm_yday - time_now->tm_yday) + std::string(" Days"); }
	//					else {
	//						bool hours = false;
	//						if ((t_episode - t_now) >= (60 * 60)) { 
	//							t_string += to_string((int)ceil((t_episode - t_now) / (60 * 60))) + std::string(" Hours"); hours = true; 
	//							int hour_calc = ceil((t_episode - t_now) / (60 * 60));
	//							t_string += " And ";
	//							t_string += to_string((int)ceil(((t_episode - t_now) - (hour_calc * 60 * 60))/(60))) + std::string(" Minutes");
	//						}
	//						if ((t_episode - t_now) <= (60 * 60)) {if (hours == true) { t_string += " And "; } t_string += to_string((int)ceil((t_episode - t_now) / (60))) + std::string(" Minutes");}
	//					}
	//				}
	//				else {
	//					t_string = "0 Seconds";
	//				}
	//				
	//				Alcubierre::Debug::Log::Msg("JATAPT", "Episode : [%s] set to publish in %s", ep.title.c_str(), t_string.c_str());
	//				//Spooled_Episodes.push_back(ep);
	//				Save_Spooled_Episodes();
	//				break;
	//			}
	//			case J_FC::EPISODE_EXISTING:
	//			{
	//				JATAPT::COMMON::J_EP ep = JATAPT::COMMON::Deserialize_Episode(data[J_PF_DATA]);
	//				Write_Episode_To_File(ep, true);
	//				break;
	//			}
	//			case J_FC::EPISODE_DELETE:
	//			{
	//				break;
	//			}
	//		}
	//	}

	//}
}

void Server::LoadEpisodes()
{
	//todo

	/*std::vector<JATAPT::COMMON::J_EP> eps = std::vector<JATAPT::COMMON::J_EP>();
	eps = JATAPT::COMMON::Serialize_Episodes(config.xml_path,config.audio_web_prefix);
	Server::Live_Episodes = eps;
	Episode_JSON_Data = JATAPT::COMMON::Packetize_Episodes(eps);*/
}

struct {
	bool operator()(const char* a, const char* b) const
	{
		return (doj::alphanum_comp(a, b) < 0);
	}
} alphanum_comp_func;

void Server::LoadFiles()
{
	const char* file_dir = config.audio_path;

	//check if directory exists
	struct stat buff;
	int result = stat(file_dir, &buff);
	if (result != 0) { Alcubierre::Exit(-666); }

	//todo generalize this for anyone to use and not just splitview / flickview
	//todo validate everything
	std::vector<char*> split_view_files = Alcubierre::File::Util::ListDir(config.audio_path);
	split_view_files = Alcubierre::File::Util::Strip_FileTypes(split_view_files, ".mp3", true);
	std::sort(split_view_files.begin(), split_view_files.end(), alphanum_comp_func);
	std::vector<char*> flick_view_files = Alcubierre::File::Util::ListDir((std::string(config.audio_path)+std::string("/Flick.View")).c_str());
	flick_view_files = Alcubierre::File::Util::Strip_FileTypes(flick_view_files, ".mp3", true);
	flick_view_files = charVector_Prefix(flick_view_files, "Flick.View/");
	std::sort(flick_view_files.begin(), flick_view_files.end(), alphanum_comp_func);
	Files = charVector_Add(split_view_files, flick_view_files);


	/*nlohmann::json j;

	for (int i = 0; i <Files.size(); i++)
	{
		j["Files"][i] = Files[i];
	}

	std::string data = j.dump();
	File_JSON_Data = new char[data.size()];
	strcpy(File_JSON_Data, data.c_str());*/

}

bool Server::Write_Episode_To_File(JATAPT::COMMON::J_EP ep, bool existing)
{
	tinyxml2::XMLDocument xml_doc;
	xml_doc.LoadFile(config.xml_path);

	tinyxml2::XMLNode* nd_Channel = xml_doc.FirstChildElement("rss")->FirstChildElement("channel");

	tinyxml2::XMLElement* ep_node;
	tinyxml2::XMLElement* title_element;
	tinyxml2::XMLElement* description_element;
	tinyxml2::XMLElement* itunes_summary;
	tinyxml2::XMLElement* itunes_subtitle;
	tinyxml2::XMLElement* itunes_episode_type;
	tinyxml2::XMLElement* enclosure;
	tinyxml2::XMLElement* guid;
	tinyxml2::XMLElement* itunes_duration;
	tinyxml2::XMLElement* publication_date;

	if (!existing)
	{
		ep_node = xml_doc.NewElement("item");
		title_element = xml_doc.NewElement("title");
		description_element = xml_doc.NewElement("description");
		itunes_summary = xml_doc.NewElement("itunes:summary");
		itunes_subtitle = xml_doc.NewElement("itunes:subtitle");
		itunes_episode_type = xml_doc.NewElement("itunes:episodeType");
		enclosure = xml_doc.NewElement("enclosure");
		guid = xml_doc.NewElement("guid");
		itunes_duration = xml_doc.NewElement("itunes:duration");
		publication_date = xml_doc.NewElement("pubDate");
	}
	else {
		for (tinyxml2::XMLElement* t1 = nd_Channel->FirstChildElement("item"); t1 != NULL; t1 = t1->NextSiblingElement())
		{
			std::string guid = t1->FirstChildElement("guid")->GetText();
			if (guid == ep.guid)
			{
				ep_node = t1;
				break;
			}
		}
		title_element = ep_node->FirstChildElement("title");
		description_element = ep_node->FirstChildElement("description");
		itunes_summary = ep_node->FirstChildElement("itunes:summary");
		itunes_subtitle = ep_node->FirstChildElement("itunes:subtitle");
		itunes_episode_type = ep_node->FirstChildElement("itunes:episodeType");
		enclosure = ep_node->FirstChildElement("enclosure");
		guid = ep_node->FirstChildElement("guid");
		itunes_duration = ep_node->FirstChildElement("itunes:duration");
		publication_date = ep_node->FirstChildElement("pubDate");
	}

	title_element->SetText(ep.title.c_str());
	description_element->SetText(ep.description.c_str());
	itunes_summary->SetText(ep.summary.c_str());
	itunes_subtitle->SetText(ep.subtitle.c_str());
	itunes_episode_type->SetText("full");

	struct stat stat_buff;
	string file_path = string(config.audio_path) + string("/") + string(ep.audio_url);
	stat(file_path.c_str(), &stat_buff);
	size_t file_size = stat_buff.st_size;

	if (!existing)
	{
		auto new_guid = xg::newGuid();
		std::stringstream stream;
		stream << new_guid;
		auto guidString = stream.str();
		guid->SetText(guidString.c_str());
	}
	else {
		guid->SetText(ep.guid.c_str());
	}


	uint8_t* file_buffer = new uint8_t[16384];
	mio::mmap_source mmap(file_path, 0, 16384);
	memcpy(file_buffer, mmap.data(), 16384);
	mmap.unmap();

	static mp3dec_t mp3d;
	mp3dec_init(&mp3d);
	mp3dec_frame_info_t info;
	short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
	/*unsigned char *input_buf; - input byte stream*/
	int samples = mp3dec_decode_frame(&mp3d, file_buffer, 16384, pcm, &info);

	int audio_size = file_size - info.frame_offset;
	int file_seconds = ((audio_size * 8) / 1024)/info.bitrate_kbps;
	int hours = floor(file_seconds / 3600);
	int minutes = floor((file_seconds - (hours * 3600))/60);
	int seconds = ((file_seconds - (hours * 3600)) - (60 * minutes));

	char* duration_string = new char[16];
	sprintf(duration_string, "%02i:%02i:%02i", hours, minutes, seconds);
	duration_string[strlen(duration_string)] = '\0';
	itunes_duration->SetText(duration_string);

	if (existing)
	{
		publication_date->SetText(ep.publication_date.c_str());
	}
	else {
		std::time_t t = std::time(0);   // get time now
		std::tm* tm_now = std::localtime(&t);
		const char* pub_time = SerializeRFC822(tm_now);
		publication_date->SetText(pub_time);
	}


	string audio_url_str = string(config.audio_web_prefix) + string(ep.audio_url);
	char* audio_url = new char[audio_url_str.size()];
	strncpy(audio_url, audio_url_str.data(), audio_url_str.size());
	audio_url[audio_url_str.size()] = '\0';

	enclosure->SetAttribute("url", audio_url);
	enclosure->SetAttribute("type", "audio/mpeg");
	enclosure->SetAttribute("length", file_size);

	ep_node->InsertEndChild(title_element);
	ep_node->InsertEndChild(description_element);
	ep_node->InsertEndChild(itunes_summary);
	ep_node->InsertEndChild(itunes_subtitle);
	ep_node->InsertEndChild(itunes_episode_type);
	ep_node->InsertEndChild(enclosure);
	ep_node->InsertEndChild(guid);
	ep_node->InsertEndChild(itunes_duration);
	ep_node->InsertEndChild(publication_date);

	nd_Channel->InsertEndChild(ep_node);

	tinyxml2::XMLError error = xml_doc.SaveFile(config.xml_path);
	//Server::LoadEpisodes();
	return true;
}

void JATAPT::SERVER::StartServer(char* config_path)
{
	mio::mmap_source config_map(config_path);
	const char* data = config_map.data();
	config_map.end();
	config = Server_Config();

	json jdata;

	try
	{
		jdata = json::parse(data);
	}
	catch (json::exception e)
	{
		Alcubierre::Debug::Log::Msg("ERROR", (char*)e.what());
		Alcubierre::Exit(-666);
	}

	std::string config_password = jdata["config"]["password"];
	int config_listen_port = jdata["config"]["listen-port"];
	std::string config_listen_interface = jdata["config"]["listen-interface"];
	std::string config_xml_path = jdata["config"]["xml-path"];
	std::string audio_path = jdata["config"]["audio-path"];
	std::string audio_web_prefix = jdata["config"]["audio-web-prefix"];
	int config_blob_size = jdata["config"]["blob-size"];
	int config_blob_count = jdata["config"]["blob-count"];
	int throttle_val = jdata["config"]["server-throttle"];
	std::string spooled_ep_path = jdata["config"]["spooled-episodes-path"];

	config.password = config_password.c_str();
	config.listen_port = config_listen_port;
	config.listen_address = config_listen_interface.c_str();
	config.xml_path = config_xml_path.c_str();
	config.blob_size = config_blob_size;
	config.blob_count = config_blob_count;
	config.audio_path = audio_path.c_str();
	config.audio_web_prefix = audio_web_prefix.c_str();
	config.throttle_val = throttle_val;
	config.spooled_path = spooled_ep_path.c_str();

	if (Alcubierre::File::Util::FileExists(config.xml_path) != true)
	{
		Alcubierre::Debug::Log::Msg("Jatapt", "xml-path : %s", config.xml_path);
		Alcubierre::Debug::Log::Msg("ERROR", "xml-path file does not exist");
		Alcubierre::Exit(-666);
	}

	Server sv;
	Alcubierre::Initialize_Core();
	Alcubierre::Net::init();
	sv.Run();

}
