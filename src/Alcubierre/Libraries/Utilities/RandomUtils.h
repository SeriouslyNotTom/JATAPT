#pragma once

#include <vector>
#include <ctime>

/// <summary>
/// Adds prefix_val to the beginning of each vector item
/// </summary>
/// <param name="vector_input">input char array vector to be prefixed</param>
/// <param name="prefix_val">char sequence to add the beginning of each vector item</param>
/// <returns>result of prefix operation</returns>
std::vector<char*> charVector_Prefix(std::vector<char*> vector_input, char* prefix_val);

/// <summary>
/// Appends append_val to the end of each vector item
/// </summary>
/// <param name="vector_input">input char array vector to be appended</param>
/// <param name="append_val">char sequence to add to the end of each vector item</param>
/// <returns>result of append operation</returns>
std::vector<char*> charVector_Append(std::vector<char*> vector_input, char* append_val);

/// <summary>
/// Adds add_val's items to vector_input and returns the combination
/// </summary>
/// <param name="vector_input">Vector to add items too</param>
/// <param name="add_val">Vector items to add to vector_input</param>
/// <returns>Combination of vector_input and add_val</returns>
std::vector<char*> charVector_Add(std::vector<char*> vector_input, std::vector<char*> add_val);

/// <summary>
/// Removes the matched sequence from the beginning or end of the input sequence
/// </summary>
/// <param name="string_input">string to be trimmed</param>
/// <param name="Trim_Sequence">Sequence to remove</param>
/// <param name="reverse">When enabled removes from the beinning of the string instead of the tail</param>
/// <returns>either the trimmed string or the unaltered input if no sequence is found</returns>
const char* char_TrimString(const char* string_input, const char* Trim_Sequence, bool reverse = false);

std::time_t tm_to_time_t(tm* time_struct);