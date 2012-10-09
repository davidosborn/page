/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <string>
#include <pwd.h> // getpwuid
#include <stdlib.h> // getenv
#include <sys/utsname.h> // uname
#include <unistd.h> // get{hostname,login,uid}
#include "../err/exception/throw.hpp" // THROW

namespace page
{
	namespace sys
	{
		std::string GetEnvVar(const std::string &);

		std::string GetCpu()
		{
			struct utsname name;
			if (uname(&name) == -1) THROW err::PlatformException<err::PosixPlatformTag>("failed to query system information");
			return name.machine;
		}
		std::string GetOs()
		{
			struct utsname name;
			if (uname(&name) == -1) THROW err::PlatformException<err::PosixPlatformTag>("failed to query system information");
			return std::string(name.sysname) + ' ' + name.release;
		}
		std::string GetHost()
		{
			char name[256];
			if (gethostname(name, sizeof name) == -1) THROW err::PlatformException<err::PosixPlatformTag>("failed to query host name");
			return name;
		}
		std::string GetUser()
		{
			const char *name = getlogin();
			if (!name) THROW err::PlatformException<err::PosixPlatformTag>("failed to query login name");
			return name;
		}
		std::string GetHome()
		{
			std::string home(GetEnvVar("HOME"));
			if (home.empty())
			{
				passwd *pw = getpwuid(getuid());
				if (!pw) THROW err::PlatformException<err::PosixPlatformTag>("failed to get home directory");
				home = pw->pw_dir;
			}
			return home;
		}
		std::string GetEnvVar(const std::string &name)
		{
			const char *value = getenv(name.c_str());
			if (!value) value = "";
			return value;
		}
	}
}
