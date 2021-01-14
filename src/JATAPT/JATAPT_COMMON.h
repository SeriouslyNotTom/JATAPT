#pragma once

#include <tinyxml2.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <steam/isteamnetworkingsockets.h>
#include <chrono>
#include <vector>
#include <map>
#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/string.h>



extern const char* Months[];
extern const char* Days[];
extern const char* Hours[];
extern const char* Minutes[];

extern std::map<char, char*> XMLSpecialChars;

namespace JATAPT
{
	namespace EPISODE
	{
		


	}
	namespace COMMON
	{
		//JATAPT_COMMAND_HEADER
		enum J_CH
		{
			AUTH_RESPONSE,
			AUTH_CHECK,
			AUTH_SUCCESS,
			AUTH_FAIL,
			DATA_TRANSFER_INIT,
			DATA_TRANSFER_BLOCK,
			DATA_TRANSFER_END,
			EPISODE_QUERY,
			EPISODE_EDIT,
			FILES_QUERY,
			SERVER_INFO
		};

		//JATAPT_PACKET_FIELD
		static const char* J_PF_HEADER = "Header";
		static const char* J_PF_PASSWORD = "Password";
		static const char* J_PF_EPISODES = "Episodes";
		static const char* J_PF_FILES = "Files";
		static const char* J_PF_QUERY = "Query";
		static const char* J_PF_OPERATION = "Operation";
		static const char* J_PF_DATA = "Data";

		//JATAPT_FIELD_COMMAND
		enum J_FC
		{
			FULL_QUERY,
			SINGLE_QUERY,
			EPISODE_NEW,
			EPISODE_EXISTING,
			EPISODE_DELETE
		};

		struct J_EP
		{
			std::string title;
			std::string description;
			std::string summary;
			std::string subtitle;
			std::string audio_url;
			std::string audio_duration;
			std::string guid;
			std::string publication_date;
		};

		template <typename S>
		void serialize(S& s, J_EP& o) {
			s.text1b(o.title);
		};

		void init_common();

		std::vector<JATAPT::COMMON::J_EP> Serialize_Episodes(const char* xml_file, const char* audio_web_prefix);
		const char* Packetize_Episode(JATAPT::COMMON::J_EP Episode);
		const char* Packetize_Episodes(std::vector<JATAPT::COMMON::J_EP> eps);

		std::vector<JATAPT::COMMON::J_EP> Deserialize_Episodes(char* JSON_DATA);
		JATAPT::COMMON::J_EP Deserialize_Episode(std::string json_data);

		bool Check_Episode_Fully_Defined(JATAPT::COMMON::J_EP Episode);

		enum JATAPTEpisodeVerifyState_
		{
			JATAPTEpisodeVerifyState_Pass = 0,
			JATAPTEpisodeVerifyState_NoTitle = 1 << 0,
			JATAPTEpisodeVerifyState_NoDescription = 1 << 1,
			JATAPTEpisodeVerifyState_NoSubtitle = 1 << 2,
			JATAPTEpisodeVerifyState_NoFile = 1 << 3,
			JATAPTEpisodeVerifyState_NoDuration = 1 << 4,
			JATAPTEpisodeVerifyState_NoPubDate = 1 << 5,
			JATAPTEpisodeVerifyState_NoGuid = 1 << 6,
			JATAPTEpisodeVerifyState_NoSummary = 1 << 7
		};

		extern int TitleDescriptionSubtitleFilePubDateSummary;
		extern int GuidDUration;

		JATAPTEpisodeVerifyState_ Verify_Episode(J_EP Episode);

		void ParseRFC822(const char* string, tm* time_struct);
		char* SerializeRFC822(tm* time_struct);

		char* ParseXMLSpecialCharacter(char* input_Sequence);

	}
}
