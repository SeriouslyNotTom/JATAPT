#pragma once

#include <string>

namespace Alcubierre
{
	namespace Tripwire
	{
		enum SEVERITY
		{
			IM_TOO_YOUNG_TO_DIE = 663,
			HURT_ME_PLENTY = 664,
			ULTRA_VIOLENCE = 665,
			NIGHTMARE = 666
		};

		extern SEVERITY default_serverity;
		extern std::string reason;

		void setDefaultAssertionSeverity(SEVERITY new_severity);

		//asserts
		//bool assert(bool check_val); //serverity is determined by "tripwire::setDefaultAssertionServerity"
		//bool assert(bool check_val, Tripwire::SEVERITY raise_level);
		//bool assert(bool check_val, Tripwire::SEVERITY raise_level, std::string msg);
		//bool assert(bool check_val, std::string msg);

		//errors
		bool error(Tripwire::SEVERITY raise_level);
		bool error(Tripwire::SEVERITY raise_level, std::string msg);
		bool error(std::string msg);

	}
}