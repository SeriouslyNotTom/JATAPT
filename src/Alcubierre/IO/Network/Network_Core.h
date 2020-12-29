#pragma once

#include <stdlib.h>

namespace Alcubierre
{
	namespace Net
	{
		void init();
		char* resolve(char* host);

		class AlcbSocket
		{
			/*int64_t* Socket_ID;
			SocketManager* Parent_Manager;
			
			enum SocketConnectionState { Unknown, Connecting, Connected, Disconnected, Listening };
			const typedef std::function<void(int64_t Socket, char* data, size_t data_len)> OnReceivedData_Callback;
			const typedef std::function<void(int64_t Socket, char* data, size_t data_len)> OnSendData_Callback;
			const typedef std::function<void(int64_t Socket, SocketConnectionState SocketState)> OnConnectionStateChange_Callback;
			
			void AddHook_OnReceivedData(OnReceivedData_Callback* CB);
			void AddHook_OnSendData(OnSendData_Callback* CB);
			void AddHook_OnConnectionStateChange(OnConnectionStateChange_Callback* CB);

			bool SendData(std::string data_string);
			bool SendData(char* data, size_t data_len);

			bool Connect(char* ip, int port);
			bool Connect();

			bool Disconnect(char* reason);
			bool Disconnect();

			bool SetPort(int port_num);
			int GetPort();

			bool SetInterface(char* address);*/

		};

		enum ALCB_SocketType{UNKNOWN,LISTEN,CONNECT};

		struct EndConnectionInfo {
			int Ping_MS;
			time_t Time_Connected;
			time_t Time_Disconnected;
		};

		class SocketManager
		{
			AlcbSocket* CreateSocket(ALCB_SocketType Socket_Type);
			//AlcbSocket* CreateSocket(Socket_Type SocketType, );
		};
		
	}
}