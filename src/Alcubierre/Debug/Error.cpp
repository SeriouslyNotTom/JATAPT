#include <Alcubierre/Debug/Error.h>

#include <sys/stat.h>
#include <Alcubierre/Debug/Log.h>
#include <Alcubierre/Alcubierre.h>

void Alcubierre::Error::CheckFile(const char* path, bool crash, int error_code, char* message)
{

	struct stat buff {};
	int result = stat(path, &buff);
	if (result == -1 | buff.st_size == 0)
	{
		Alcubierre::Debug::Log::Msg("ERROR", message, path);
		if (crash) { Alcubierre::Exit(error_code);}
	}

}