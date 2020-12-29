#include <Alcubierre/Libraries/Utilities/RandomUtils.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <map>
#include <time.h>
#include <csignal>
#include <Alcubierre/Debug/Log.h>

using namespace std;

/// <summary>
/// Adds prefix_val to the beginning of each vector item
/// </summary>
/// <param name="vector_input">input char array vector to be prefixed</param>
/// <param name="prefix_val">char sequence to add the beginning of each vector item</param>
/// <returns>result of prefix operation</returns>
std::vector<char*> charVector_Prefix(std::vector<char*> vector_input, char* prefix_val)
{
	std::vector<char*> new_vec = std::vector<char*>();
	for (int i = 0; i < vector_input.size(); i++)
	{
		char* item = vector_input[i];
		char* new_item = new char[strlen(item) + strlen(prefix_val)];
		strcpy(new_item, prefix_val);
		strcat(new_item, item);
		new_vec.push_back(new_item);
	}
	return new_vec;
}

/// <summary>
/// Appends append_val to the end of each vector item
/// </summary>
/// <param name="vector_input">input char array vector to be appended</param>
/// <param name="append_val">char sequence to add to the end of each vector item</param>
/// <returns>result of append operation</returns>
std::vector<char*> charVector_Append(std::vector<char*> vector_input, char* append_val)
{
	std::vector<char*> new_vec = std::vector<char*>();
	for (int i = 0; i < vector_input.size(); i++)
	{
		char* item = vector_input[i];
		char* new_item = new char[strlen(item) + strlen(append_val)];
		strcpy(new_item, item);
		strcat(new_item, append_val);
		new_vec.push_back(new_item);
	}
	return new_vec;
}

/// <summary>
/// Adds add_val's items to vector_input and returns the combination
/// </summary>
/// <param name="vector_input">Vector to add items too</param>
/// <param name="add_val">Vector items to add to vector_input</param>
/// <returns>Combination of vector_input and add_val</returns>
std::vector<char*> charVector_Add(std::vector<char*> vector_input, std::vector<char*> add_val)
{
	std::vector<char*> new_vec = vector_input;
	for (int i = 0; i < add_val.size(); i++)
	{
		new_vec.push_back(add_val[i]);
	}
	return new_vec;
}

const char* char_TrimString(const char* string_input, const char* Trim_Sequence, bool reverse)
{
	int trimSeqLen = strlen(Trim_Sequence);
	char* check_str = new char[trimSeqLen];

	if (reverse)
	{
		strncpy(check_str, string_input + (strlen(string_input) - trimSeqLen), trimSeqLen);
		check_str[trimSeqLen] = '\0';
		if (std::string(check_str) == std::string(Trim_Sequence))
		{
			char* final_str = new char[strlen(string_input) - trimSeqLen];
			memcpy(final_str, string_input, strlen(string_input) - trimSeqLen);
			final_str[strlen(string_input)-trimSeqLen] = '\0';
			return final_str;
		}
	}
	else
	{
		strncpy(check_str, string_input, trimSeqLen);
		check_str[trimSeqLen] = '\0';
		if (std::string(check_str) == std::string(Trim_Sequence))
		{
			char* final_str = new char[strlen(string_input) - trimSeqLen];
			strncpy(final_str, string_input + trimSeqLen, strlen(final_str));
			return final_str;
		}
	}

	return string_input;
}

std::time_t tm_to_time_t(tm* time_struct)
{
	//no idea if this works
	std::time_t time = 0;

	std::time_t seconds_in_year = 60 * 60 * 24 * 365;
	time = time + (seconds_in_year * ((time_struct->tm_year)-70));
	time = time + ((time_struct->tm_yday) * (60 * 60 * 24));
	time = time + ((time_struct->tm_hour) * (60 * 60));
	time = time + ((time_struct->tm_min) * 60);
	time = time + (time_struct->tm_sec);

	//don't ask me where 1000800 came from
	//i assume it's because of epoch
	return time + (time_t)1000800;
}
