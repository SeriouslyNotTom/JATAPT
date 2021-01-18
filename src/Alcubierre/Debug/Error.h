
namespace Alcubierre
{
	namespace Error
	{
		void CheckFile(const char* path, bool crash = true, int error_code = -666, char* message = "File: %s does not exist, Is not accessible (check permissions) or is empty");
	}
}