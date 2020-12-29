#include <stdlib.h>

class File
{
	bool file_returned = false;
	std::string full_path = "";
	std::string name = "null";
	int size_bytes = 0;
	char* file_bytes = 0;
};

class Provider
{
	virtual File getFile(char* file_name) { return File(); };
};

class Provider_Base
{
	
};