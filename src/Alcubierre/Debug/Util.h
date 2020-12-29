#if defined(_WIN32)
	#include <windows.h>
	#include <debugapi.h>
#endif

#include <Alcubierre/Alcubierre.h>

namespace Alcubierre
{
	namespace Debug
	{
		static void Break()
		{
			if (Alcubierre::Debug_Mode == true)
			{
			#if defined(_WIN32)
				DebugBreak();
			#endif
			}
		}
	}
}
