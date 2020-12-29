#include <Alcubierre/Libraries/Debug/Metrics.h>
#include <glad/glad.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif // _WIN32

int Debug_Metrics::getMemoryUsage()
{
#ifdef _WIN32
	PROCESS_MEMORY_COUNTERS mem_info;
	GetProcessMemoryInfo(GetCurrentProcess(), &mem_info, sizeof(mem_info));
	return	mem_info.WorkingSetSize;
#endif
#ifndef _WIN32
	return 0;
#endif // !_WIN32
}
