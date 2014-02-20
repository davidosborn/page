/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
