#include <sched.h> // sched_yield

#include "../err/Exception.hpp"

namespace page
{
	namespace sys
	{
		void Sleep()
		{
			if (sched_yield() == -1)
				THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to yield processor") <<
					boost::errinfo_api_function("sched_yield")))
		}
	}
}
