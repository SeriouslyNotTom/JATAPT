#include <Alcubierre/Debug/Log.h>
#include <map>
#include <spdlog/spdlog.h>

enum FGColorMap
{
	fgBlack = 30,
	fgRed = 31,
	fgGreen = 32,
	fgYellow = 33,
	fgBlue = 34,
	fgMagenta = 35,
	fgCyan = 36,
	fgWhite = 37,
	fgBright_Black = 90,
	fgBright_Red = 91,
	fgBright_Green = 92,
	fgBright_Yellow = 93,
	fgBright_Blue = 94,
	fgBright_Magenta = 95,
	fgBright_Cyan = 96,
	fgBright_White = 97
};

//void Alcubierre::Debug::Log::setChannelColor(char* channel, ChannelColors color)
//{
//
//}

void Alcubierre::Debug::Log::Msg(char* channel, char* fmt, ...)
{
	//const char* base = "\u001b[95m[LOG::%s]\u001b[36m %s\u001b[0m";
	const char* base = "%s";
	char* buff = new char[8192];
	va_list lst;
	va_start(lst, fmt);
	vsprintf(buff, fmt, lst);
	va_end(lst);
	char* finalbuff = new char[strlen(base) + strlen(buff)];
	sprintf(finalbuff, base, buff);
	//fprintf(stdout, finalbuff);
	if (std::string(channel) == std::string("ERROR")) { spdlog::error(finalbuff); }
	else {
		spdlog::info(finalbuff);
	}
	
}