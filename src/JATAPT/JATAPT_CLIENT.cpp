#include <JATAPT/JATAPT_CLIENT.h>
#include <JATAPT/JATAPT_COMMON.h>
#include <Alcubierre/Engine.h>

#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <nlohmann/json.hpp>
#include <Alcubierre/IO/FIleUtils.h>
#include <Alcubierre/IO/Network/Network_Core.h>
#include <imguidatechooser/imguidatechooser.h>
#include <string>
#include <stdlib.h>
#include <Alcubierre/Libraries/Utilities/RandomUtils.h>
#include <fstream>
#include <jticon.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define strtk_no_tr1_or_boost
#include <datetime_utils.hpp>
#include <ctime>

using namespace JATAPT::CLIENT;
using namespace JATAPT::COMMON;

using json = nlohmann::json;

const char* JATAPT::CLIENT::settings_location = "settings.jatapt";

J_CS JATAPT::CLIENT::State;
CLIENT_VARS JATAPT::CLIENT::Vars;
JATAPT_GUI JATAPT::CLIENT::Gui;
ImGui_Handler JATAPT::CLIENT::ImGuiInstance;
JATAPT_NET_CLIENT JATAPT::CLIENT::Net_Client;

#pragma region JATAPT_GUI_DEFINITION

void JATAPT_GUI::DrawConnectWindow()
{

	ImGui::SetNextWindowSize(ImVec2(500, 135));
	ImGui::SetNextWindowPos(ImVec2(((Vars.window_size.x) / 2) - (500 / 2), (Vars.window_size.y / 2) - (135 / 2)));
	ImGui::Begin("Connect To Server", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::InputText("Server Address", input_url, 1024);
	ImGui::InputText("Password", input_pass, 1024, ImGuiInputTextFlags_Password);

	ImGui::SameLine();
	ImGui::Checkbox("Remember?", &Vars.remember_pass);

	if (State == J_CS::INIT)
	{
		
		
		if (ImGui::Button("Connect"))
		{
			Vars.password = Gui.input_pass;
			if (strlen(Gui.input_url)>0)
			{
				json settings;

				settings["last-url"] = Gui.input_url;
				settings["auto-connect"] = Vars.auto_connect;
				if (Vars.remember_pass)
				{
					settings["last-pass"] = Vars.password;
				}

				std::string json_dump = settings.dump();

				std::ofstream outfile;
				outfile.open(settings_location);
				outfile.clear();
				outfile << json_dump.c_str();
				outfile.close();
			}
			
			
			Net_Client.Run();
		}
		if (Vars.remember_pass) { ImGui::Checkbox("Auto Connect?", &Vars.auto_connect); }
	}

	if(connect_status!=""){ ImGui::TextColored(ImVec4(1.0,0.0,0.0,1.0),connect_status); }

	ImGui::End();
}

void JATAPT_GUI::DrawMainList()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(Vars.window_size);
	ImGui::Begin("JATAPT", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
	if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Episodes"))
		{
			if (ImGui::BeginChild("Episodes", ImVec2(Vars.window_size.x, (Vars.editing==true ? 290 : 425)), true))
			{
				//episode list
				ImGui::Columns(3, "EpisodeColumns");
				ImGui::Separator();
				ImGui::TextColored(ImVec4(1.0F, 0.239F, 0.874F, 1.0F), "Title"); ImGui::NextColumn();
				ImGui::TextColored(ImVec4(1.0F, 0.239F, 0.874F, 1.0F), "Duration"); ImGui::NextColumn();
				ImGui::TextColored(ImVec4(1.0F, 0.239F, 0.874F, 1.0F), "Date"); ImGui::NextColumn();
				ImGui::Separator();

				ImGui::SetColumnWidth(0, 320);
				ImGui::SetColumnWidth(1, 70);
				ImGui::SetColumnWidth(2, 300);

				for (int i = 0; i < Vars.EPS.size(); i++)
				{
					J_EP ep = Vars.EPS[i];
					if (ImGui::Selectable(ep.title.c_str(), selected_ep == i, ImGuiSelectableFlags_SpanAllColumns))
					{
						selected_ep = i;
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s", ep.episode_file.file_name.c_str());
						ImGui::EndTooltip();
					}
					ImGui::NextColumn();
					ImGui::Text(std::to_string(ep.episode_file.file_duration_seconds).c_str()); ImGui::NextColumn();
					//ImGui::Text(ep.publication_date.c_str()); ImGui::NextColumn();
				}

				ImGui::EndChild();
			}

			if (!Vars.editing)
			{

				if (ImGui::Button("New"))
				{
					ClearDraft();

					Vars.editing = true;
				}


				ImGui::SameLine();

				/// <summary>
				/// load up episode
				/// </summary>
				if (ImGui::Button("Edit") & selected_ep != -1 & selected_ep <= Vars.EPS.size())
				{

					J_EP ep = Vars.EPS[selected_ep];

					draft_ep = FillDraft(ep);
					selected_ep_guid = (char*)ep.guid.c_str();

					Vars.editing = true;

				}
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(200, 0, 0, 255));
				if (ImGui::Button("DELETE"))
				{
					ImGui::OpenPopup("Delete?");
				}
				ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::Separator();
				ImGui::SameLine();
				ImGui::PopItemWidth();
				if (ImGui::Button("Refresh"))
				{
					Net_Client.GetEps();
				}

				if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Are you sure you want to delete this?.\n\r");
					ImGui::Separator();

					if (ImGui::Button("Yes", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}

			if (Vars.editing)
			{
				ImGui::Separator();
				DrawEditWindow();
			}

			ImGui::TextColored(ImVec4(0.71f, 0.00f, 1.00f, 1.00f), "%s %s (%s %s %s %s) By Tom Gough", PROJECT_NAME_READABLE, PROJECT_VER, PROJECT_OS, PROJECT_BUILD_TYPE,PROJECT_VER_TYPE, PROJECT_BUILD_DATE);
			//ImGui::Text("Selected EP: %i", Gui.selected_ep);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Audio Files"))
		{
			DrawFileListing();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();

	}

	ImGui::End();
}

void JATAPT_GUI::DrawEditWindow()
{

	if (ImGui::BeginChild("Edit Window", ImVec2(Vars.window_size.x, 150), true))
	{

		ImGui::Spacing();

		if (ImGui::BeginCombo("Audio File", draft_ep.file))
		{
			for (int i = 0; i < Vars.Files.size(); i++)
			{
				bool selected = (draft_ep.file == Vars.Files[i]);
				if (ImGui::Selectable(Vars.Files[i], selected))
				{
					draft_ep.file = Vars.Files[i];
				}
				if (selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}

		ImGui::InputText("Title", draft_ep.title, 1024);
		ImGui::InputText("Description", draft_ep.description, 1024);
		ImGui::InputText("Subtitle", draft_ep.subtitle, 1024);
		ImGui::SameLine(); ImGui::TextColored(ImVec4(strlen(draft_ep.subtitle) > 256 ? 1.0f : 0.0f, strlen(draft_ep.subtitle) > 256 ? 0.0f : 1.0f, 0.0f, 1.0f), "Chars: %i (256 max)", strlen(draft_ep.subtitle));

		ImGui::PushItemWidth(50);
		if (ImGui::BeginCombo(":", draft_ep.hour))
		{
			for (int i = 0; i <= 23; i++)
			{
				bool selected = (draft_ep.hour == Hours[i]);
				if (ImGui::Selectable(Hours[i],selected))
				{
					draft_ep.hour = Hours[i];
				}
				if (selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();

		ImGui::PushItemWidth(50);
		if (ImGui::BeginCombo("##Minute", draft_ep.minute))
		{
			for (int i = 0; i <= 59; i++)
			{
				bool selected = (draft_ep.minute == Minutes[i]);
				if (ImGui::Selectable(Minutes[i], selected))
				{
					draft_ep.minute = Minutes[i];
				}
				if (selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();

		ImGui::PushItemWidth(100);
		ImGui::DateChooser("##Publish Date", draft_ep.time, "%d/%m/%Y",false);

		ImGui::SameLine();
		if (ImGui::Button("Now"))
		{
			const time_t t_now = std::time(0);
			struct tm* time = localtime(&t_now);
			draft_ep.hour = Hours[time->tm_hour];
			draft_ep.minute = Minutes[time->tm_min];
			draft_ep.time = *time;
		}
		ImGui::SameLine();
		if (ImGui::Button("Sunday"))
		{
			draft_ep.time.tm_hour = 0;
			draft_ep.time.tm_min = 0;
			draft_ep.time.tm_sec = 1;
			if (draft_ep.time.tm_wday != 0)
			{
				int delta = draft_ep.time.tm_wday;
				time_t t_d = tm_to_time_t(&draft_ep.time);
				t_d += (time_t)((7-delta) * (60 * 60 * 24));
				draft_ep.time = *localtime(&t_d);
			}
			draft_ep.hour = Hours[0];
			draft_ep.minute = Minutes[0];
			draft_ep.time.tm_sec = 0;
		}

		ImGui::Spacing();
		if (ImGui::Button("Save"))
		{
			J_EP ep = FillEpisode(draft_ep);
			Gui.InputVerifyState = Verify_Episode(ep);

			JATAPTEpisodeVerifyState_ state;
			state = (JATAPTEpisodeVerifyState_)Gui.InputVerifyState;
			if (state & JATAPTEpisodeVerifyState_NoEpTitle | state & JATAPTEpisodeVerifyState_NoEpDescription | state & JATAPTEpisodeVerifyState_NoFileName | state & JATAPTEpisodeVerifyState_NoEpSubtitle)
			{
				ImGui::OpenPopup("Invalid Input");
			}
			else {
				Net_Client.SendEpisode(ep);
				ClearDraft();
				Vars.editing = false;
				Net_Client.GetEps();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ClearDraft();
			Vars.editing = false;
		}

		ImGui::SetNextWindowSize(ImVec2(350, 150));
		if (ImGui::BeginPopupModal("Invalid Input", nullptr ,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
		{
			JATAPTEpisodeVerifyState_ state;
			state = (JATAPTEpisodeVerifyState_)Gui.InputVerifyState;

			if (state & JATAPTEpisodeVerifyState_NoFileName) { ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Missing File How did you even this"); }
			if (state & JATAPTEpisodeVerifyState_NoEpTitle) { ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Missing Title, \nPlease input a title for the episode"); }
			if (state & JATAPTEpisodeVerifyState_NoEpDescription) { ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Missing Description, \nPlease input a description for the episode"); }
			if (state & JATAPTEpisodeVerifyState_NoEpSubtitle) { ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Missing Subtitle, \n Please input a subtitle for the episode"); }

			ImGui::Spacing();
			ImGui::Separator();

			if (ImGui::Button("Ok", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		ImGui::EndChild();

	}
}

void JATAPT_GUI::DrawFileListing()
{
	if (ImGui::BeginChild("Files", ImVec2(Vars.window_size.x, 465), true))
	{
		//episode list
		ImGui::Columns(1, "FilesColumns");
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0F, 0.239F, 0.874F, 1.0F), "Name"); ImGui::NextColumn();
		ImGui::Separator();

		for (int i = 0; i < Vars.Files.size(); i++)
		{
			ImGui::Text(Vars.Files[i]);
		}

		ImGui::EndChild();
		/*ImGui::Separator();
		ImGui::Spacing();*/
		/*if (ImGui::Button("Upload File"))
		{
			char* file = Alcubierre::File::Util::BrowseFileSingle("Mp3 File Please", "(Mp3 Files .mp3)\0*.mp3");
			if (file != NULL)
			{
				State = J_CS::TRANSFERRING;
			}
			else {

			}

		}*/
	}
}

void JATAPT_GUI::DrawTransferWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 75));
	ImGui::SetNextWindowPos(ImVec2(((Vars.window_size.x) / 2) - (300 / 2), (Vars.window_size.y / 2) - (75 / 2)));
	ImGui::Begin("Transfering", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("38682482/24682848");
	ImGui::Text("35%");
	ImGui::Text("3.34mb/s");

	ImGui::End();
}

void JATAPT_GUI::ImGuiFrameStart()
{
	switch (State)
	{
	case J_CS::EDITING:
	case J_CS::CONNECTED:
		DrawMainList();
		if (!Vars.first_query)
		{
			Vars.first_query = true;
			Net_Client.GetEps();
			Net_Client.GetFiles();
		}
		break;

	case J_CS::INIT:
	case J_CS::CONNECTING:
	case J_CS::AUTHENTICATING:
		DrawConnectWindow();
		break;

	case J_CS::TRANSFERRING:
		DrawTransferWindow();
		break;
	}
};

void JATAPT_GUI::ImGuiFrameEnd() {};

void JATAPT_GUI::ClearDraft()
{
	draft_ep.title = new char[1024];
	memset(draft_ep.title, 0x00, 1024);
	draft_ep.description = new char[1024];
	memset(draft_ep.description, 0x00, 1024);
	draft_ep.subtitle = new char[1024];
	memset(draft_ep.subtitle, 0x00, 1024);
	selected_ep = -1;
	std::time_t t = std::time(0);   // get time now
	std::tm* tm_now = std::localtime(&t);
	draft_ep.time = *tm_now;
	draft_ep.hour = "23";
	draft_ep.minute = "59";
	draft_ep.guid = "";
	if (Vars.Files.size() > 0)
	{
		draft_ep.file = Vars.Files[Vars.Files.size() - 1];
	}
}

DRAFT_DATA JATAPT_GUI::FillDraft(JATAPT::COMMON::J_EP episode)
{

	//TODO: fix this

	DRAFT_DATA draft = {};
	draft.title = new char[1024];
	strcpy(draft.title, episode.title.c_str());
	draft.description = new char[1024];
	strcpy(draft.description, episode.description.c_str());
	draft.subtitle = new char[1024];
	strcpy(draft.subtitle, episode.subtitle.c_str());
	draft.file = new char[1024];
	strcpy(draft.file, episode.audio_url.c_str());
	struct tm* time;
	ParseRFC822(episode.publication_date.c_str(),time);
	draft.hour = Hours[time->tm_hour];
	draft.minute = Minutes[time->tm_min];
	draft.time = *time;
	draft.guid = new char[episode.guid.size()];
	strcpy(draft.guid, episode.guid.c_str());
	return draft;
}

JATAPT::COMMON::J_EP JATAPT::CLIENT::JATAPT_GUI::FillEpisode(DRAFT_DATA draft)
{

	//TODO: fix this

	JATAPT::COMMON::J_EP ep;

	ep.title = draft.title;
	ep.description = draft.description;
	ep.summary = draft.description;
	ep.subtitle = draft.subtitle;
	ep.episode_file.file_name = draft.file;
	ep.guid = draft.guid;
	int hours = 0;
	int minutes = 0;
	sscanf((std::string(draft_ep.hour) + std::string(":") + std::string(draft_ep.minute)).c_str(), "%d:%d", &hours, &minutes);
	draft.time.tm_hour = hours;
	draft.time.tm_min = minutes;
	draft.time.tm_sec = 0;
	ep.publication_date = SerializeRFC822(&draft.time);
	return ep;
}

JATAPT_GUI::JATAPT_GUI()
{
	input_url = new char[1024];
	memset(input_url, 0x00, 1024);
	strcpy(input_url, "");
	input_pass = new char[1024];
	memset(input_pass, 0x00, 1024);
	connect_status = "";
	InputVerifyState = 0;
	ClearDraft();
;}

#pragma endregion JATAPT_GUI_DEFINITION

#pragma region JATAPT_NET_CLIENT_DEFINITION

void JATAPT_NET_CLIENT::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_None:
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		char* buff = new char[1024];
		sprintf(buff, "Could not connect: %s", pInfo->m_info.m_szEndDebug);
		Gui.connect_status = buff;
		m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		m_hConnection = k_HSteamNetConnection_Invalid;
		State = J_CS::INIT;
		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
	{
		Gui.connect_status = "Connecting...";
		break;
	}

	case k_ESteamNetworkingConnectionState_Connected:
	{
		Alcubierre::Debug::Log::Msg("Network", "Connected!");
		Gui.connect_status = "Authenticating...";
		m_hConnection = pInfo->m_hConn;
	}
	}
}

void JATAPT_NET_CLIENT::ProcessPacket(const char* data)
{
	auto pkt = json::parse(data);
	J_CH Header = J_CH(pkt[J_PF_HEADER]);
	switch (Header)
	{
		case J_CH::AUTH_SUCCESS:
		{
			Alcubierre::Debug::Log::Msg("Network", "AUTH SUCCESS!");
			Vars.authenticated = true;
			State = J_CS::CONNECTED;
			break;
		}
		case J_CH::AUTH_FAIL:
		{
			Alcubierre::Debug::Log::Msg("Network", "AUTH FAIL");
			Gui.connect_status = "Wrong Password";
			Cleanup();
			State = J_CS::INIT;
			break;
		}
		case J_CH::EPISODE_QUERY:
		{
			if (pkt[J_PF_QUERY] == "FULLSET RESPONSE")
			{
				std::string data = pkt[J_PF_DATA];
				Episode_JSON_Data = new char[data.size()];
				strcpy(Episode_JSON_Data, data.c_str());
				Vars.EPS = JATAPT::COMMON::Deserialize_Episodes(Episode_JSON_Data);
			}
			break;
		}
		case J_CH::AUTH_CHECK:
		{
			State = J_CS::AUTHENTICATING;
			TryAuth();
			break;
		}
		case J_CH::FILES_QUERY:
		{
			if (pkt[J_PF_QUERY] == "FULLSET RESPONSE")
			{
				std::string data = pkt[J_PF_DATA];
				Files_JSON_Data = new char[data.size()];
				strcpy(Files_JSON_Data, data.c_str());
				nlohmann::json j = nlohmann::json::parse(Files_JSON_Data);
				Vars.Files = std::vector<char*>();
				for (std::string element : j["Files"])
				{
					char* file = new char[element.size()];
					strcpy(file, element.c_str());
					Vars.Files.push_back(file);
				}
			}
		}
	}
}

void JATAPT_NET_CLIENT::Cleanup()
{
	if (m_hConnection != k_ESteamNetworkingConnectionState_None)
		m_pInterface->CloseConnection(m_hConnection, 0, "User Closed Program", 0);
}

void JATAPT_NET_CLIENT::Poll()
{
	PollIncoming();
	m_pInterface->RunCallbacks(this);
}

void JATAPT_NET_CLIENT::PollIncoming()
{
	ISteamNetworkingMessage* incoming = nullptr;
	int numMsg = m_pInterface->ReceiveMessagesOnConnection(m_hConnection, &incoming, 1);
	if (numMsg == 0) { return; }
	if (numMsg > 0)
	{
		if (incoming->m_cbSize > 0)
		{
			std::string incoming_data;
			incoming_data.assign((const char*)incoming->m_pData, incoming->m_cbSize);
			const char* data = incoming_data.c_str();
			Alcubierre::Debug::Log::Msg("Network", "Got Packet");
			ProcessPacket(data);
		}
	}

}

void JATAPT_NET_CLIENT::Run()
{
	memset(addr, 0x00, 255);
	addr = Alcubierre::Net::resolve(Gui.input_url);
	serverAddr.ParseString(addr);
	serverAddr.m_port = 6665;
	m_pInterface = SteamNetworkingSockets();
	char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
	serverAddr.ToString(szAddr, sizeof(szAddr), true);
	Alcubierre::Debug::Log::Msg("Network", "Connecting to : %s", szAddr);
	m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 0, nullptr);
	State = J_CS::CONNECTING;
	if (m_hConnection == k_HSteamNetConnection_Invalid)
	{
		Alcubierre::Debug::Log::Msg("ERROR", "FAILED TO CONNECT");
	}
}

void JATAPT_NET_CLIENT::ConnectToServer(char* con_addr, int con_port)
{
	addr = Alcubierre::Net::resolve(con_addr);
	serverAddr.ParseString(addr);
	serverAddr.m_port = con_port;
	m_pInterface = SteamNetworkingSockets();
	char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
	serverAddr.ToString(szAddr, sizeof(szAddr), true);
	Alcubierre::Debug::Log::Msg("Network", "Connecting to : %s", szAddr);
	m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 0, nullptr);
	State = J_CS::CONNECTING;
	if (m_hConnection == k_HSteamNetConnection_Invalid)
	{
		Alcubierre::Debug::Log::Msg("ERROR", "FAILED TO CONNECT");
	}
}

void JATAPT_NET_CLIENT::TryAuth()
{
	//strcpy(Gui.connect_status, "Authenticating...");
	json j;
	j[J_PF_HEADER] = J_CH::AUTH_RESPONSE;
	j[J_PF_PASSWORD] = Vars.password;
	std::string jdat = j.dump();
	m_pInterface->SendMessageToConnection(m_hConnection, jdat.c_str(), (uint32)jdat.length(), k_nSteamNetworkingSend_Reliable, nullptr);
}

void JATAPT_NET_CLIENT::GetEps()
{
	json j;
	j[J_PF_HEADER] = J_CH::EPISODE_QUERY;
	j[J_PF_QUERY] = "FULLSET";
	std::string jdat = j.dump();
	m_pInterface->SendMessageToConnection(m_hConnection, jdat.c_str(), (uint32)jdat.length(), k_nSteamNetworkingSend_Reliable, nullptr);
}

void JATAPT_NET_CLIENT::GetFiles()
{
	json j;
	j[J_PF_HEADER] = J_CH::FILES_QUERY;
	j[J_PF_QUERY] = "FULLSET";
	std::string jdat = j.dump();
	m_pInterface->SendMessageToConnection(m_hConnection, jdat.c_str(), (uint32)jdat.length(), k_nSteamNetworkingSend_Reliable, nullptr);
}

void JATAPT_NET_CLIENT::SendEpisode(JATAPT::COMMON::J_EP Episode)
{

	//TODO: [JATAPT] fix send episode

	/*JATAPTEpisodeVerifyState_ verif = Verify_Episode(Episode);
	if (verif & JATAPTEpisodeVerifyState_NoEpTitle | verif & JATAPTEpisodeVerifyState_NoEpDescription | verif & JATAPTEpisodeVerifyState_NoEpSummary | verif & JATAPTEpisodeVerifyState_NoSubtitle | verif & JATAPTEpisodeVerifyState_NoFile | verif & JATAPTEpisodeVerifyState_NoPubDate) { return; }

	json j;
	j[J_PF_HEADER] = J_CH::EPISODE_EDIT;
	std::string data;
	json j2;

	if (Episode.guid.size() > 0)
	{
		j[J_PF_OPERATION] = J_FC::EPISODE_EXISTING;
		j2["guid"] = Episode.guid;
	}
	else {
		j[J_PF_OPERATION] = J_FC::EPISODE_NEW;
	}

	j2["title"] = Episode.title;
	j2["description"] = Episode.description;
	j2["summary"] = Episode.summary;
	j2["subtitle"] = Episode.subtitle;
	j2["audio_url"] = Episode.audio_url;
	j2["publication_date"] = Episode.publication_date;

	data = j2.dump();
	j[J_PF_DATA] = data;
	std::string packet_data = j.dump();
	m_pInterface->SendMessageToConnection(m_hConnection, packet_data.c_str(), (uint32)packet_data.length(), k_nSteamNetworkingSend_Reliable, nullptr);*/
}

#pragma endregion JATAPT_NET_CLIENT_DEFINITION

void WindowCloseCallback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
	Net_Client.Cleanup();
}

void WindowCallback(Alcubierre::Engine::Window::WindowInstance* window)
{
	window->window_width = Vars.window_size.x;
	window->window_height = Vars.window_size.y;
	window->glfw_monitor = NULL;
	window->window_title = "JATAPT | Jordy and Tom's Awesome Podcast Tool";
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, false);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void window_iconify_callback(GLFWwindow* window, int iconified)
{
	if (iconified)
	{
		Vars.render = false;
		Alcubierre::Debug::Log::Msg("Jatapt", "Window Minimized.. halting renderer");
	}
	else
	{
		Vars.render = true;
		Alcubierre::Debug::Log::Msg("Jatapt", "Window Maximized.. resuming renderer");
	}
}

void JATAPT::CLIENT::Init(int argc, char* argv[])
{
	Alcubierre::Accept_Arguments(argc, *argv);
	Alcubierre::Initialize_Core();
	Alcubierre::Engine::Initialize();

	Vars.window_size = ImVec2(640, 500);

	Alcubierre::Engine::Window::WindowCreationCallback window_cb = static_cast<Alcubierre::Engine::Window::WindowCreationCallback>(&WindowCallback);
	Alcubierre::Engine::SpawnWindow(&window_cb);

	int x, y, chan;
	unsigned char* data = stbi_load_from_memory(JT_256x256_8BIT_png, JT_256x256_8BIT_png_size, &x, &y,&chan,4);

	GLFWimage icon[1];
	icon[0].pixels = data;
	icon[0].height = x;
	icon[0].width = y;
	glfwSetWindowIcon(Alcubierre::Engine::PrimaryWindow->glfw_window, 1, icon);

	glfwSetWindowIconifyCallback(Alcubierre::Engine::PrimaryWindow->glfw_window, window_iconify_callback);
	glfwSetWindowCloseCallback(Alcubierre::Engine::PrimaryWindow->glfw_window, &WindowCloseCallback);
	glfwMakeContextCurrent(Alcubierre::Engine::PrimaryWindow->glfw_window);

	ImGuiInstance = ImGui_Handler();
	ImGuiInstance.AcceptWindow(Alcubierre::Engine::PrimaryWindow);
	ImGuiInstance.Init();

	Gui = JATAPT_GUI();
	ImGuiInstance.AddImGuiRenderableOBJ(&Gui);

	//load settings
	Vars.settings_exists = false;
	Vars.settings_exists = Alcubierre::File::Util::FileExists(settings_location);
	if (Vars.settings_exists)
	{
		char* settings_data = Alcubierre::File::Util::Load_File_memmap(settings_location);
		if (sizeof(settings_data) > 1)
		{
			try {
				json settings = json::parse(settings_data);
				Vars.password = settings["last-pass"];
				Vars.last_url = settings["last-url"];
				Vars.auto_connect = false;
				Vars.auto_connect = settings["auto-connect"];
			}
			catch (nlohmann::json::exception e)
			{
				Alcubierre::Debug::Log::Msg("ERROR", "Settings file @ %s is Corrupt", settings_location);
				Vars.settings_exists = false;
			}
		}
	}

	Vars.EPS = std::vector<JATAPT::COMMON::J_EP>();

	Alcubierre::Renderer::Initialize();
	ProgramLoop();
}

void JATAPT::CLIENT::NetworkCleanup()
{
	Vars.Files.clear();
	Vars.EPS.clear();
	Vars.editing = false;
	Vars.authenticated = false;
	Net_Client.Cleanup();
}

void JATAPT::CLIENT::ProgramLoop()
{
	Alcubierre::Net::init();

	glfwSwapInterval(1);
	Vars.render = true;

	if (Vars.settings_exists)
	{
		if (Vars.last_url.size() > 0) { Gui.input_url = (char*)Vars.last_url.c_str(); }
		if (Vars.password.size() > 0) { Gui.input_pass = (char*)Vars.password.c_str(); }
		if (Vars.auto_connect == true) { Net_Client.Run(); }
	}

	glfwShowWindow(Alcubierre::Engine::PrimaryWindow->glfw_window);

	while (!glfwWindowShouldClose(Alcubierre::Engine::PrimaryWindow->glfw_window))
	{
		if (State != J_CS::INIT)
		{
			Net_Client.Poll();
		}
		//if (glfwGetTime() >= ts + frame_freq)

			//ts = glfwGetTime();
			glfwPollEvents();
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.8f);

			if (Vars.render == true)
			{
				ImGuiInstance.OnRender();
			}
			glfwSwapBuffers(Alcubierre::Engine::PrimaryWindow->glfw_window);

	}

	glfwTerminate();
	Alcubierre::Exit(0);
}
