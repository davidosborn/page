#include <string>

#include <boost/optional.hpp>

#include <pwd.h> // getpwuid
#include <stdlib.h> // getenv
#include <sys/utsname.h> // uname
#include <unistd.h> // get{hostname,login,uid}

#include "../err/Exception.hpp"

namespace page
{
	namespace sys
	{
		std::string GetCpu()
		{
			struct utsname name;
			if (uname(&name) == -1)
				THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get system information") <<
					boost::errinfo_api_function("uname")))
			return name.machine;
		}

		std::string GetOs()
		{
			struct utsname name;
			if (uname(&name) == -1)
				THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get system information") <<
					boost::errinfo_api_function("uname")))
			return std::string(name.sysname) + ' ' + name.release;
		}

		std::string GetHostname()
		{
			char name[256];
			if (gethostname(name, sizeof name) == -1)
				THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get hostname") <<
					boost::errinfo_api_function("gethostname")))
			return name;
		}

		std::string GetUsername()
		{
			const char *name = getlogin();
			if (!name)
				THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get login name") <<
					boost::errinfo_api_function("getlogin")))
			return name;
		}

		std::string GetHome()
		{
			const char *home = getenv("HOME");
			if (!home)
			{
				passwd *pw = getpwuid(getuid());
				if (!pw)
					THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get home directory") <<
						boost::errinfo_api_function("getpwuid")))
				home = pw->pw_dir;
			}
			return home;
		}

		boost::optional<std::string> GetEnvVar(const std::string &name)
		{
			const char *value = getenv(name.c_str());
			return value ? boost::in_place(value) : boost::none;
		}
	}
}
