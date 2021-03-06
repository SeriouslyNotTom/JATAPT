#pragma once

#include <JATAPT/JATAPT_COMMON.h>
#include <Alcubierre/Alcubierre.h>
#include <sys/stat.h>
#include <tinyxml2.h>
#include <Alcubierre/Libraries/Utilities/RandomUtils.h>
#include <nlohmann/json.hpp>
#include <map>
#include <signal.h>
#include <Alcubierre/Debug/Log.h>
#include <pugixml.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace tinyxml2;
using namespace JATAPT::COMMON;


//i might be ashamed of having these variables, but i don't go around digging up your backyard now do i?
std::map<std::string, int> DayMap = { {"Sun",0}, {"Mon",1}, {"Tue", 2}, {"Wed",3}, {"Thu",4}, {"Fri", 5}, {"Sat",6} };
std::map<std::string, int> MonthMap = { {"Jan", 0}, {"Feb", 1}, {"Mar", 2}, {"Apr", 3}, {"May", 4}, {"Jun", 5}, {"Jul", 6}, {"Aug", 7}, {"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11} };

const char* Months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char* Days[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char* Hours[24] = { "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23" };
const char* Minutes[60] = { "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59" };

int seconds_in_year = (60 * 60) * 24 * 365;

std::map<char, char*> XMLSpecialChars =
{
	{'&',"&amp;"},
	{'<',"&lt;"},
	{'>',"&gt;"},
	{'"',"&quot;"},
	{39, "&apos;"}
};

int JATAPT::COMMON::JATAPTEpisodeVerifyState_TitleDescriptionSubtitleSummaryFileNameFilePathPubDate =

	JATAPTEpisodeVerifyState_NoEpTitle |
	JATAPTEpisodeVerifyState_NoEpDescription |
	JATAPTEpisodeVerifyState_NoEpSubtitle |
	JATAPTEpisodeVerifyState_NoEpSummary |

	JATAPTEpisodeVerifyState_NoFileName |
	JATAPTEpisodeVerifyState_NoFilePath |

	JATAPTEpisodeVerifyState_NoPubDate;

//const char* JATAPT::COMMON::Packetize_Episode(JATAPT::COMMON::J_EP Episode)
//{
//	nlohmann::json j;
//
//	j[Episode.guid]["title"] = Episode.title;
//	j[Episode.guid]["description"] = Episode.description;
//	j[Episode.guid]["summary"] = Episode.summary;
//	j[Episode.guid]["subtitle"] = Episode.subtitle;
//	j[Episode.guid]["audio_url"] = Episode.audio_url;
//	j[Episode.guid]["audio_duration"] = Episode.audio_duration;
//	j[Episode.guid]["guid"] = Episode.guid;
//	j[Episode.guid]["publication_date"] = Episode.publication_date;
//
//	std::string json_data = j.dump();
//	char* json_str = new char[json_data.size()];
//	strcpy(json_str, json_data.c_str());
//	return json_str;
//}

//const char* JATAPT::COMMON::Packetize_Episodes(std::vector<JATAPT::COMMON::J_EP> eps)
//{
//	Alcubierre::Debug::Log::Msg("Jatapt", "Packetizing Episodes..");
//	nlohmann::json j;
//
//	for (int i = 0; i < eps.size(); i++)
//	{
//		J_EP ep = eps[i];
//		j["episodes"][i]["title"] = ep.title;
//		j["episodes"][i]["description"] = ep.description;
//		j["episodes"][i]["summary"] = ep.summary;
//		j["episodes"][i]["subtitle"] = ep.subtitle;
//		j["episodes"][i]["audio_url"] = ep.audio_url;
//		j["episodes"][i]["audio_duration"] = ep.audio_duration;
//		j["episodes"][i]["guid"] = ep.guid;
//		j["episodes"][i]["publication_date"] = ep.publication_date;
//	}
//
//	std::string json_data = j.dump();
//	Alcubierre::Debug::Log::Msg("Jatapt", "Done");
//	return json_data.c_str();
//}

//std::vector<JATAPT::COMMON::J_EP> JATAPT::COMMON::Deserialize_Episodes(char* JSON_DATA)
//{
//	std::vector<JATAPT::COMMON::J_EP> eps;
//	Document document;
//	document.Parse(JSON_DATA);
//
//
//
//	return eps;
//}

//std::vector<JATAPT::COMMON::J_EP> JATAPT::COMMON::Deserialize_Episodes(char* JSON_DATA)
//{
//	std::vector<JATAPT::COMMON::J_EP> eps;
//	nlohmann::json k = nlohmann::json::parse(JSON_DATA);
//
//	for (auto& element : k["episodes"])
//	{
//		J_EP ep;
//		std::string title = element["title"];
//		ep.title = title;
//
//		std::string description = element["description"];
//		ep.description = description;
//
//		std::string summary = element["summary"];
//		ep.summary = summary;
//
//		std::string subtitle = element["subtitle"];
//		ep.subtitle = subtitle;
//
//		std::string audio_url = element["audio_url"];
//		ep.audio_url = audio_url;
//
//		std::string audio_duration = element["audio_duration"];
//		ep.audio_duration = audio_duration;
//
//		std::string guid = element["guid"];
//		ep.guid = guid;
//
//		std::string publication_date = element["publication_date"];
//		ep.publication_date = publication_date;
//
//		eps.push_back(ep);
//	}
//
//	return eps;
//}

//JATAPT::COMMON::J_EP JATAPT::COMMON::Deserialize_Episode(std::string json_data)
//{
//	auto element = nlohmann::json::parse(json_data);
//
//	J_EP ep;
//	std::string title = element["title"];
//	ep.title = title;
//
//	std::string description = element["description"];
//	ep.description = description;
//
//	std::string summary = element["summary"];
//	ep.summary = summary;
//
//	std::string subtitle = element["subtitle"];
//	ep.subtitle = subtitle;
//
//	std::string audio_url = element["audio_url"];
//	ep.audio_url = audio_url;
//
//	std::string publication_date = element["publication_date"];
//	ep.publication_date = publication_date;
//
//	if(element.find("guid")!=element.end())
//	{
//		std::string guid = element["guid"];
//		ep.guid = guid;
//	}
//
//	return ep;
//}

//bool JATAPT::COMMON::Check_Episode_Fully_Defined(JATAPT::COMMON::J_EP Episode)
//{
//	//i know i can use reflection to do this but if yall know any other way that is less lines than this; please hit me up. -Tom
//	if (Episode.title == "") { return false; }
//	if (Episode.description == "") { return false; }
//	if (Episode.summary == "") { return false; }
//	if (Episode.subtitle == "") { return false; }
//	if (Episode.audio_url == "") { return false; }
//	if (Episode.audio_duration == "") { return false; }
//	if (Episode.guid == "") { return false; }
//	if (Episode.publication_date == "") { return false; }
//	return true;
//}

JATAPT::COMMON::JATAPTEpisodeVerifyState_ JATAPT::COMMON::Verify_Episode(J_EP Episode)
{
	int state = JATAPTEpisodeVerifyState_Pass;

	//text stuff
	if (Episode.title == "" | (Episode.title.size()<=0)) { state = state | JATAPTEpisodeVerifyState_NoEpTitle; }
	if (Episode.description == "" | (Episode.description.size()<=0)) { state = state | JATAPTEpisodeVerifyState_NoEpDescription; }
	if (Episode.summary == "" | (Episode.summary.size() <=0)) { state = state | JATAPTEpisodeVerifyState_NoEpSummary; }
	if (Episode.subtitle == "" | (Episode.subtitle.size() <=0)) { state = state | JATAPTEpisodeVerifyState_NoEpSubtitle; }

	//file stuff
	if (Episode.episode_file.file_name == "" | (Episode.episode_file.file_name.size() <= 0)) { state = state | JATAPTEpisodeVerifyState_NoFileName; }
	//if (Episode.episode_file.file_duration_seconds == 0 | (Episode.episode_file.file_duration_seconds==-1)) { state = state | JATAPTEpisodeVerifyState_NoFileDuration; }
	if (Episode.episode_file.file_path == "" | (Episode.episode_file.file_path.size() <=0)) { state = state | JATAPTEpisodeVerifyState_NoFilePath; }
	if (Episode.episode_file.file_size == 0 | (Episode.episode_file.file_size == -1)) { state = state | JATAPTEpisodeVerifyState_NoFileSize; }

	//other stuff
	if (Episode.guid == "" | (Episode.guid.size() <= 0)) { state = state | JATAPTEpisodeVerifyState_NoGuid; }
	if (Episode.episode_publication_time == 0 | (Episode.episode_publication_time == -1)) { state = state | JATAPTEpisodeVerifyState_NoPubDate; }

	return (JATAPTEpisodeVerifyState_)state;
}

/// <summary>
/// try to parse a string date i.e. ("Thu, 10 May 2020 00:00:01 GMT") into a time struct
/// date has to be in RFC822 format obviously
/// </summary>
/// <param name="string">RFC822 Date/Time string</param>
/// <param name="time_struct">time struct to be filled with parsed time</param>
void JATAPT::COMMON::ParseRFC822(const char* string, tm* time_struct)
{

	char* s_day = new char[3];
	char* s_month = new char[3];
	char* s_time = new char[8];
	char* s_timezone = new char[3];
	int i_day = 0;
	int i_year = 0;

	sscanf(std::string(string).c_str(), "%s %d %s %d %s %s", s_day, &i_day, s_month, &i_year, s_time, s_timezone);
	s_day[3] = '\0';
	s_month[3] = '\0';

	//Alcubierre::Debug::Log::Msg("DEBUG","T: %s %d %s %d %s %s\r\n",s_day,i_day,s_month,i_year,s_time,s_timezone);

	time_struct->tm_year = i_year - 1900;
	time_struct->tm_mday = i_day;
	time_struct->tm_wday = DayMap[std::string(s_day)];
	time_struct->tm_mon = MonthMap[std::string(s_month)];

	
	int t_hour = 0;
	int t_min = 0;
	int t_sec = 0;

	//i kid you not wrapping this input in a std::string makes it work. if you don't sometimes it just doesnt
	sscanf(std::string(s_time).c_str(), "%d:%d:%d", &t_hour, &t_min, &t_sec);

	time_struct->tm_hour = t_hour;
	time_struct->tm_min = t_min;
	time_struct->tm_sec = t_sec;

	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int ydays = 0;
	for (int i = 0; i < (time_struct->tm_mon); i++)
	{
		ydays = ydays + (daysInMonth[i]);
	}
	ydays = ydays+ (time_struct->tm_mday);
	time_struct->tm_yday = ydays;
}

/// <summary>
/// Convert a tm time struct into an rfc822 date/time string
/// </summary>
/// <param name="time_struct">tm time struct to be converted</param>
/// <returns>returned string of rfc822 date/time</returns>
char* JATAPT::COMMON::SerializeRFC822(tm* time_struct)
{
	char* str = new char[1024];
	int chars = sprintf(str, "%s, %i %s %i %02i:%02i:%02i GMT",Days[time_struct->tm_wday], time_struct->tm_mday, Months[time_struct->tm_mon], (time_struct->tm_year+1900),time_struct->tm_hour,time_struct->tm_min, time_struct->tm_sec);
	char* output = new char[chars];
	strncpy(output, str, chars);
	output[chars] = '\0';
	return output;
}

/// <summary>
/// replace special characters (& < > " ') from input string into their xml safe counter parts
/// </summary>
/// <param name="input_Sequence">string to make safe</param>
/// <returns>string that has been made XML safe</returns>
char* JATAPT::COMMON::ParseXMLSpecialCharacter(char* input_Sequence)
{
	char* final_str = "";
	size_t input_size = strlen(input_Sequence);
	bool run = false;

	for (int i = 0; i <= input_size; i++)
	{
		char test_char = input_Sequence[i];
		if (test_char == '<' | test_char == '>' | test_char == '"' | test_char == '&' | test_char == 39)
		{
			char* exit_seq = XMLSpecialChars[test_char];

			if (!(strlen(final_str)>0))
			{
				final_str = new char[i];
				strncpy(final_str, input_Sequence, i);
				final_str[i] = '\0';
				run = true;
			}

			char* temp_seq = new char[strlen(final_str) + strlen(exit_seq)];
			strcpy(temp_seq, final_str);
			strcpy(temp_seq+strlen(final_str), exit_seq);
			final_str = temp_seq;
		}
	}
	if (run == false)
	{
		return input_Sequence;
	}

	return final_str;
}

time_t JATAPT::COMMON::RFC822_to_time_t(const char* RFC_string)
{
	//very bad
	tm time_struct{};
	ParseRFC822(RFC_string, &time_struct);
	time_t final_time = tm_to_time_t(&time_struct);
	return final_time;
}

void JATAPT::COMMON::init_common()
{

}
