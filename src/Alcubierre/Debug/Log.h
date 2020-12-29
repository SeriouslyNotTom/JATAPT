#pragma once

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdarg.h>
//
//static enum ChannelColors { Red = 31, Blue };

namespace Alcubierre
{
	namespace Debug
	{
		namespace Log
		{
			//void setChannelColor(char* channel, ChannelColors color);

			void Msg(char* channel, char* fmt, ...);
			
		}
	}
}
