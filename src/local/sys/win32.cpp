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

#include <algorithm> // find
#include <string>
#include <vector>
#include <shlobj.h> // SHGetFolderPath
#include <windows.h>
#include "../err/exception/throw.hpp" // THROW
#include "../util/sstream.hpp" // make_ostringstream
#include "../util/string.hpp" // Trim
#include "../util/win32/string.hpp" // Narrow, Native, String

// HACK: SHGFP_TYPE_CURRENT may not be defined
#if defined __MINGW32__ && _WIN32_IE < 0x0500
#	define SHGFP_TYPE_CURRENT 0
#endif

namespace page
{
	namespace sys
	{
		std::string GetCpu()
		{
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("Hardware\\Description\\System\\CentralProcessor\\0"), 0,
				KEY_READ, &key) != ERROR_SUCCESS) THROW err::PlatformException<err::Win32PlatformTag>("failed to open registry key");
			std::vector<BYTE> buffer(32);
			DWORD type, size = buffer.size();
			LONG result;
			while ((result = RegQueryValueEx(key, TEXT("ProcessorNameString"), 0,
				&type, &*buffer.begin(), &size)) == ERROR_MORE_DATA) buffer.resize(size);
			RegCloseKey(key);
			if (result != ERROR_SUCCESS) THROW err::PlatformException<err::Win32PlatformTag>("failed to query registry value");
			if (type != REG_SZ) THROW err::PlatformException<err::Win32PlatformTag>("registry value type mismatch");
			return util::TrimLeading(util::win32::Narrow(util::win32::String(
				reinterpret_cast<LPTSTR>(&*buffer.begin()), std::find(
				reinterpret_cast<LPTSTR>(&*buffer.begin()),
				reinterpret_cast<LPTSTR>(&*buffer.end()), TEXT('\0')))));
		}
		std::string GetOs()
		{
			OSVERSIONINFO vi = {sizeof vi};
			if (!GetVersionEx(&vi)) THROW err::PlatformException<err::Win32PlatformTag>("failed to get Windows version");
			const char *base = "Windows";
			switch (vi.dwPlatformId)
			{
				case VER_PLATFORM_WIN32_NT:
				switch (vi.dwMajorVersion)
				{
					case 4:
					switch (vi.dwMinorVersion)
					{
						case 0: base = "Windows NT"; break;
					}
					break;
					case 5:
					switch (vi.dwMinorVersion)
					{
						case 0: base = "Windows 2000"; break;
						case 1: base = "Windows XP"; break;
						case 2: base = "Windows Server 2003"; break;
					}
					break;
					case 6:
					switch (vi.dwMinorVersion)
					{
						case 0: base = "Windows Vista"; break;
					}
					break;
				}
				break;
				case VER_PLATFORM_WIN32_WINDOWS:
				switch (vi.dwMajorVersion)
				{
					case 4:
					switch (vi.dwMinorVersion)
					{
						case 0: base = "Windows 95"; break;
						case 10: base = "Windows 98"; break;
						case 90: base = "Windows Me"; break;
					}
					break;
				}
				break;
			}
			MAKE_OSTRINGSTREAM_1(char)//auto ss(util::make_ostringstream<char>());
			ss << base <<
				" " << vi.dwMajorVersion <<
				"." << vi.dwMinorVersion <<
				"." << LOWORD(vi.dwBuildNumber) <<
				" " << util::win32::Narrow(util::win32::String(vi.szCSDVersion));
			return ss.str();
		}
		std::string GetHost()
		{
			std::vector<TCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1, TEXT('\0'));
			DWORD size = buffer.size();
			while (!GetComputerName(&*buffer.begin(), &size))
			{
				if (GetLastError() != ERROR_BUFFER_OVERFLOW)
					THROW err::PlatformException<err::Win32PlatformTag>("failed to get host name");
				buffer.resize(size + 1);
			}
			return util::win32::Narrow(util::win32::String(buffer.begin(), buffer.begin() + size));
		}
		std::string GetUser()
		{
			std::vector<TCHAR> buffer(32);
			DWORD size = buffer.size();
			while (!GetUserName(&*buffer.begin(), &size))
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
					THROW err::PlatformException<err::Win32PlatformTag>("failed to get user name");
				buffer.resize(size);
			}
			return util::win32::Narrow(util::win32::String(buffer.begin(), buffer.begin() + size - 1));
		}
		std::string GetHome()
		{
			std::vector<TCHAR> buffer(MAX_PATH);
			if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, &*buffer.begin())))
				THROW err::PlatformException<err::Win32PlatformTag>("failed to get application data directory");
			return util::win32::Narrow(util::win32::String(&*buffer.begin()));
		}
		std::string GetEnvVar(const std::string &name)
		{
			util::win32::String tname(util::win32::Native(name));
			std::vector<TCHAR> buffer(32);
			for (;;)
			{
				DWORD result = GetEnvironmentVariable(tname.c_str(), &*buffer.begin(), buffer.size());
				if (!result)
				{
					THROW GetLastError() == ERROR_ENVVAR_NOT_FOUND ?
						err::PlatformException<err::Win32PlatformTag>("environment variable not found") :
						err::PlatformException<err::Win32PlatformTag>("failed to get environment variable");
				}
				if (result <= buffer.size()) break;
				buffer.resize(result);
			}
			return util::win32::Narrow(util::win32::String(&*buffer.begin()));
		}
	}
}
