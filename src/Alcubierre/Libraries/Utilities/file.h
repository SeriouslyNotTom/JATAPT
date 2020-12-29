#pragma once

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <mio/mmap.hpp>
#include <algorithm>
#include <fcntl.h>
#ifndef WIN32
	#include <sys/uio.h>
#endif LINUX
#include <cstring>

class util_file
{
public:
	static char* LoadFile_ifstream(const char* file_path)
	{
		try
		{
			struct stat buff;
			stat(file_path, &buff);
			std::ifstream FileStream(file_path, std::ios_base::binary);
			FileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			char* buffer = new char[buff.st_size];
			buffer[buff.st_size] = '\0';
			FileStream.read(buffer, buff.st_size);
			//debug stuff
			//fprintf(stdout, "\x1B[36m-------------------------------\n%s\n%s\n-------------------------------\n\033[0m", file_path, buffer);
			return buffer;
		}
		catch (std::ifstream::failure e)
		{
			fprintf(stdout, "FAILED TO READ IFSTREAM: %s, REASON: %s, CODE: %i\n", file_path, e.what(),e.code());
		}
	}
	static char* LoadFile_memorymap(const char* file_path)
	{
		mio::mmap_source file_map(file_path);
		
		char* data = new char[file_map.size()];
		data[file_map.size()] = '\0';
		memcpy(data, file_map.data(), file_map.size());
		return data;
	}
};