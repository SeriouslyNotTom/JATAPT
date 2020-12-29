#include <Alcubierre/Debug/Tripwire/tripwire.h>

using namespace Alcubierre::Tripwire;

SEVERITY Alcubierre::Tripwire::default_serverity = SEVERITY::NIGHTMARE;

void Exit_Func(SEVERITY sev)
{
	if (sev == SEVERITY::NIGHTMARE)
	{
		exit(sev);
		abort();
	}
}

bool process_failure(SEVERITY sev)
{
	return true;
}

void Alcubierre::Tripwire::setDefaultAssertionSeverity(SEVERITY new_severity)
{
	Tripwire::default_serverity = new_severity;
}

//bool Alcubierre::Tripwire::assert(bool check_val)
//{
//	if (check_val == true)
//	{
//		return process_failure(default_serverity);
//	}
//	return false;
//}

bool Alcubierre::Tripwire::error(SEVERITY sev, std::string msg)
{
	return process_failure(sev);
}