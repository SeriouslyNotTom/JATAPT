#pragma once

#include <tinyxml2.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <steam/isteamnetworkingsockets.h>
#include <chrono>
#include <vector>
#include <map>
#include <cereal/archives/binary.hpp>
#include <sstream>
#include <cereal/types/string.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/vector.hpp>

extern const char* Months[];
extern const char* Days[];
extern const char* Hours[];
extern const char* Minutes[];

extern int seconds_in_year;

extern std::map<char, char*> XMLSpecialChars;

namespace JATAPT
{
	namespace COMMON
	{

		struct J_FILE
		{
			std::string file_name;
			std::string file_path;
			unsigned int file_size;
			int file_duration_seconds;

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(file_name, file_path, file_size);
			}
		};

		struct J_FILE_SET
		{
			std::vector<J_FILE> file_set;
			int file_count;

			template <class Archive>
			void serialize(Archive& ar)
			{
				ar(file_set,file_count);
			}
		};

		struct J_EP
		{
			std::string guid;

			std::string title;
			std::string description;
			std::string summary;
			std::string subtitle;

			J_FILE episode_file;

			time_t episode_publication_time;

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(guid, title, description, summary, subtitle, episode_file, episode_publication_time);
			}
		};

		struct J_EP_SET
		{
			std::vector<J_EP> ep_set;
			int episode_count;

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(ep_set);
			}
		};

		
		//verification stuff
		//-------------------------------------------------------------------
		bool Check_Episode_Fully_Defined(JATAPT::COMMON::J_EP Episode);

		enum JATAPTEpisodeVerifyState_
		{
			JATAPTEpisodeVerifyState_Pass = 0,

			JATAPTEpisodeVerifyState_NoEpTitle = 1 << 0,
			JATAPTEpisodeVerifyState_NoEpDescription = 1 << 1,
			JATAPTEpisodeVerifyState_NoEpSubtitle = 1 << 2,
			JATAPTEpisodeVerifyState_NoEpSummary = 1 << 3,

			JATAPTEpisodeVerifyState_NoFileName = 1 << 4,
			JATAPTEpisodeVerifyState_NoFilePath = 1 << 5,
			//JATAPTEpisodeVerifyState_NoFileDuration = 1 << 6,
			JATAPTEpisodeVerifyState_NoFileSize = 1 << 7,

			JATAPTEpisodeVerifyState_NoPubDate = 1 << 8,
			JATAPTEpisodeVerifyState_NoGuid = 1 << 9
		};

		extern int JATAPTEpisodeVerifyState_TitleDescriptionSubtitleSummaryFileNameFilePathPubDate;

		JATAPTEpisodeVerifyState_ Verify_Episode(J_EP Episode);

		//parsing utils
		//---------------------------------------------------------------------
		void ParseRFC822(const char* string, tm* time_struct);
		char* SerializeRFC822(tm* time_struct);
		char* ParseXMLSpecialCharacter(char* input_Sequence);
		time_t RFC822_to_time_t(const char* RFC_string);

		void init_common();
	}
}
