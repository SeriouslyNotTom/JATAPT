#pragma once

#include <Alcubierre/Libraries/Debug/ImGui_Handler.h>
#include <imgui.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingsockets.h>
#include <JATAPT/JATAPT_COMMON.h>
#include <time.h>

namespace JATAPT
{
	namespace CLIENT
	{

		class JATAPT_NET_CLIENT : public ISteamNetworkingSocketsCallbacks
		{
		private:
			ISteamNetworkingSockets* m_pInterface;
			HSteamNetConnection m_hConnection;
			SteamNetworkingIPAddr serverAddr;
			char* Episode_JSON_Data;
			char* Files_JSON_Data;
			char* addr = new char[255];

			virtual void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) override;
			void PollIncoming();
			void ProcessPacket(const char* data);
		public:
			//network commands
			void TryAuth();
			void GetEps();
			void GetFiles();
			void SendEpisode(JATAPT::COMMON::J_EP Episode);

			//lifecycle
			void Poll();
			void Cleanup();
			void Run();
			void ConnectToServer(char* con_addr, int con_port);
		};

		struct DRAFT_DATA
		{
			tm time;
			const char* hour;
			const char* minute;
			char* file;
			char* title;
			char* description;
			char* subtitle;
			char* guid;
		};

		class JATAPT_GUI : public ImGui_Render
		{
		public:
			char* input_url;
			char* input_pass;
			char* connect_status;

			int selected_ep;
			char* selected_ep_guid;

			int InputVerifyState;

			DRAFT_DATA draft_ep;

			void DrawConnectWindow();
			void DrawEditWindow();
			void DrawMainList();
			void DrawFileListing();
			void DrawPopup();
			void DrawTransferWindow();
			void ImGuiFrameStart();
			void ImGuiFrameEnd();
			void ClearDraft();

			DRAFT_DATA FillDraft(JATAPT::COMMON::J_EP episode);
			JATAPT::COMMON::J_EP FillEpisode(DRAFT_DATA draft);

			JATAPT_GUI();
		};

		//JATAPT CLIENT STATE
		enum J_CS { INIT, CONNECTING, CONNECTED, AUTHENTICATING, TRANSFERRING, EDITING };

		struct CLIENT_VARS
		{
			ImVec2 window_size;
			bool render;
			bool editing;
			bool authenticated;
			std::vector<JATAPT::COMMON::J_EP> EPS;
			std::vector<char*> Files;
			bool first_query = false;
			std::string password;
			std::string last_url;
			bool remember_pass;
			bool settings_exists;
			bool auto_connect;
		};

		extern J_CS State;
		extern CLIENT_VARS Vars;
		extern JATAPT_GUI Gui;
		extern ImGui_Handler ImGuiInstance;
		extern JATAPT_NET_CLIENT Net_Client;

		extern const char* settings_location;

		void ProgramLoop();
		void Init(int argc, char* argv[]);
		void NetworkCleanup();
	};
};