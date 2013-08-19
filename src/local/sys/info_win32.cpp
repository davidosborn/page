#include <algorithm> // find
#include <sstream> // ostringstream
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <shlobj.h> // SHGetFolderPath
#include <windows.h>

// HACK: SHGFP_TYPE_CURRENT may not be defined
#if defined __MINGW32__ && _WIN32_IE < 0x0500
#	define SHGFP_TYPE_CURRENT 0
#endif

#include "../err/Exception.hpp"
#include "../util/locale/convert.hpp" // Convert
#include "../util/string/operations.hpp" // Trim
#include "../util/string/StringBuilder.hpp"

namespace page
{
	namespace sys
	{
		std::string GetCpu()
		{
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("Hardware\\Description\\System\\CentralProcessor\\0"), 0,
				KEY_READ, &key) != ERROR_SUCCESS)
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to open registry key") <<
						boost::errinfo_api_function("RegOpenKeyEx")))

			std::vector<BYTE> buffer(32);
			DWORD type, size = buffer.size();
			LONG result;
			while ((result = RegQueryValueEx(key, TEXT("ProcessorNameString"), 0,
				&type, &*buffer.begin(), &size)) == ERROR_MORE_DATA) buffer.resize(size);
			RegCloseKey(key);
			if (result != ERROR_SUCCESS)
				THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to query registry value") <<
					boost::errinfo_api_function("RegQueryValueEx")))
			if (type != REG_SZ)
				THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("registry value type mismatch") <<
					boost::errinfo_api_function("RegQueryValueEx")))

			std::basic_string<TCHAR> s(
				reinterpret_cast<LPTSTR>(&*buffer.begin()),
				std::find(
					reinterpret_cast<LPTSTR>(&*buffer.begin()),
					reinterpret_cast<LPTSTR>(&*buffer.end()),
					TEXT('\0')));

			return util::Convert<char>(util::TrimLeading(s));
		}

		std::string GetOs()
		{
			OSVERSIONINFO vi = {sizeof vi};
			if (!GetVersionEx(&vi))
				THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to get Windows version") <<
					boost::errinfo_api_function("GetVersionEx")))
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
						case 0:  base = "Windows 95"; break;
						case 10: base = "Windows 98"; break;
						case 90: base = "Windows Me"; break;
					}
					break;
				}
				break;
			}
			return util::StringBuilder() <<
				base <<
				" " << vi.dwMajorVersion <<
				"." << vi.dwMinorVersion <<
				"." << LOWORD(vi.dwBuildNumber) <<
				" " << util::Convert<char>(vi.szCSDVersion);
		}

		std::string GetHostname()
		{
			std::vector<TCHAR> buffer(MAX_COMPUTERNAME_LENGTH + 1, TEXT('\0'));
			DWORD size = buffer.size();
			while (!GetComputerName(&*buffer.begin(), &size))
			{
				if (GetLastError() != ERROR_BUFFER_OVERFLOW)
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to get computer name") <<
						boost::errinfo_api_function("GetComputerName")))
				buffer.resize(size + 1);
			}
			return util::Convert<char>(
				std::basic_string<TCHAR>(buffer.begin(), buffer.begin() + size));
		}

		std::string GetUsername()
		{
			std::vector<TCHAR> buffer(32);
			DWORD size = buffer.size();
			while (!GetUserName(&*buffer.begin(), &size))
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to get user name") <<
						boost::errinfo_api_function("GetUserName")))
				buffer.resize(size);
			}
			return util::Convert<char>(
				std::basic_string<TCHAR>(buffer.begin(), buffer.begin() + size - 1));
		}

		std::string GetHome()
		{
			std::vector<TCHAR> buffer(MAX_PATH);
			if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, &*buffer.begin())))
				THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to get \"Application Data\" directory") <<
					boost::errinfo_api_function("SHGetFolderPath")))
			return util::Convert<char>(&*buffer.begin());
		}

		boost::optional<std::string> GetEnvVar(const std::string &name)
		{
			auto tname(util::Convert<TCHAR>(name));
			std::vector<TCHAR> buffer(32);
			for (;;)
			{
				DWORD result = GetEnvironmentVariable(tname.c_str(), &*buffer.begin(), buffer.size());
				if (!result)
				{
					if (GetLastError() == ERROR_ENVVAR_NOT_FOUND)
						return boost::none;

					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to get environment variable") <<
						boost::errinfo_api_function("GetEnvironmentVariable")))
				}
				if (result <= buffer.size()) break;
				buffer.resize(result);
			}
			return boost::in_place(util::Convert<char>(&*buffer.begin()));
		}
	}
}
