#include <string>
#include <stdlib.h>

class util_convert
{
public:
	static std::string format_size(int bytes)
	{
		float size;
		size = bytes;
		std::string output;
		
		if (size >= 1024)
		{
			size = size / 1024;
			if (size >= 1024)
			{
				size = size / 1024;
				if (size >= 1024)
				{
					size = size / 1024;

					return std::string(std::to_string(size) + "GB");
				}
				return std::string(std::to_string(size) + "MB");
			}
			return std::string(std::to_string(size) + "KB");
		}
		return std::string(std::to_string(size) + "B");
	}
};