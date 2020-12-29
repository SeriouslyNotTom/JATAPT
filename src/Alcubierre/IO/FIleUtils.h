#pragma once

#include <vector>
#include <sys/stat.h>

enum AlcubierreListDirFlags_
{
	AlcubierreListDirFlags_None						= 0,		//Returns only File names in immediate directory with no recursion or folder names
	AlcubierreListDirFlags_Recurse					= 1 << 0,	//Drill down into subdirectories for results
	AlcubierreListDirFLags_AppendSubdirectoryPrefix = 1 << 1,	//Prefix the subdirectory(s) name to the returned result. has no affect when AlcubierreListDirFlags_FullPath is specified
	AlcubierreListDirFlags_ReturnFolders			= 1 << 2,	//Return folders in the results
	AlcubierreListDirFlags_NoFiles					= 1 << 3,	//Don't return files in the results
	AlcubierreListDirFlags_FullPath					= 1 << 4	//Return Full path of result i.e: "C:/folder1/folder2/result"
};

typedef int AlcubierreListDirFlags;

namespace Alcubierre
{
	namespace File
	{
		namespace Util
		{
			char* BrowseFileSingle(char* Title, char* filter);
			std::vector<char*> ListDir(const char* Path, AlcubierreListDirFlags dir_flags = 0);

			bool FileExists(const char* Path, struct stat* stat_out = nullptr);

			char* Load_File_memmap(const char* path);
			bool Save_File_memmap(const char* path, char* data);

			/// <summary>
			/// Strips out Vector elements with file types matching the input parameter "File_Type"
			/// </summary>
			/// <param name="input_vector">Vector to have strings ripped from</param>
			/// <param name="File_Type">file type string to strip must be in the format of ".filetype" including the dot (.)</param>
			/// <param name="invert">Keep only file type instead of removing it, can be left blank (default = false)</param>
			/// <returns></returns>
			std::vector<char*> Strip_FileTypes(std::vector<char*> input_vector, const char* File_Type, bool invert = false);
		}
	}
}