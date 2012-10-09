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

#include <algorithm> // find_if, replace
#include <vector>
#include <shlwapi.h> // PathIsRelative
#include <windows.h>
#include "../../err/exception/throw.hpp" // THROW
#include "../../util/win32/string.hpp" // Narrow, Native, String
#include "../file.hpp" // ListDirCallback

namespace page
{
	namespace sys
	{
		namespace
		{
			// directory recursion
			void WalkNextDir(const std::string &root, const std::string &base, const std::string &name, const ListDirCallback &cb, const WalkDirRecurseFilter &recurse)
			{
				std::string path(CatPath(base, name));
				std::string absPath(CatPath(root, path));
				if (IsDir(absPath) && (!recurse || recurse(absPath)))
					ListDir(absPath, std::bind(WalkNextDir, root, path, std::placeholders::_1, cb, recurse));
				else cb(path);
			}
		}

		// character types
		bool IsSeparator(char c)
		{
			return c == '\\' || c == '/';
		}
		bool IsWildcard(char c)
		{
			switch (c)
			{
				case '*':
				case '?':
				return true;
			}
			return false;
		}
		bool IsReserved(char c)
		{
			switch (c)
			{
				case ':':
				case '"':
				case '<':
				case '>':
				case '|':
				return true;
			}
			return IsWildcard(c);
		}

		// path validation
		bool PathExists(const std::string &path)
		{
			// NOTE: handles wildcard characters
			// HACK: limited to MAX_PATH
			WIN32_FIND_DATA fd;
			HANDLE findState = FindFirstFile(util::win32::Native(path).c_str(), &fd);
			if (findState != INVALID_HANDLE_VALUE) FindClose(findState);
			return findState != INVALID_HANDLE_VALUE;
		}
		bool IsFile(const std::string &path)
		{
			// HACK: limited to MAX_PATH
			DWORD attrib = GetFileAttributes(util::win32::Native(path).c_str());
			return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
		}
		bool IsDir(const std::string &path)
		{
			// HACK: limited to MAX_PATH
			DWORD attrib = GetFileAttributes(util::win32::Native(path).c_str());
			return attrib != INVALID_FILE_ATTRIBUTES && attrib & FILE_ATTRIBUTE_DIRECTORY;
		}
		bool IsAbsPath(const std::string &path)
		{
			return !PathIsRelative(util::win32::Native(path.substr(0, MAX_PATH - 1)).c_str());
		}

		// path conversion
		std::string NormPath(const std::string &path)
		{
			if (path.empty()) return path;
			// expand environment variables
			std::vector<TCHAR> expBuffer(path.size() + 2);
			DWORD expSize;
			while ((expSize = ExpandEnvironmentStrings(util::win32::Native(path).c_str(),
				&*expBuffer.begin(), expBuffer.size())) > expBuffer.size()) expBuffer.resize(expSize);
			if (!expSize) THROW err::PlatformException<err::Win32PlatformTag>("failed to expand environment variables");
			std::string normPath(util::win32::Narrow(util::win32::String(
				expBuffer.begin(), expBuffer.begin() + expSize - 1)));
			// check for invalid characters
			if (std::find_if(normPath.begin(), normPath.end(), IsInvalid) != normPath.end())
				THROW err::Exception<err::PathTag>("path contains reserved characters");
			// replace forward slashes with backslashes
			std::replace(normPath.begin(), normPath.end(), '/', '\\');
			// remove leading and trailing backslashes
			normPath.erase(0, normPath.find_first_not_of('\\'));
			normPath.erase(normPath.find_last_not_of('\\') + 1);
			// merge adjacent backslashes
			for (std::string::size_type sep(std::string::npos);
				(sep = normPath.find("\\\\", sep + 2)) != std::string::npos;)
				normPath.erase(sep + 1, normPath.find_first_not_of('\\', sep + 2) - sep - 1);
			return normPath;
		}
		std::string CatPath(const std::string &path1, const std::string &path2)
		{
			if (path1.empty()) return path2;
			if (path2.empty()) return path1;
			return path1 + '\\' + path2;
		}

		// path extraction
		std::string BaseName(const std::string &path)
		{
			std::string::const_iterator iter(std::find_if(path.begin(), path.end(), IsSeparator));
			return iter == path.end() ? path : path.substr(iter - path.begin() + 1);
		}
		std::string DirName(const std::string &path)
		{
			std::string::const_iterator iter(std::find_if(path.begin(), path.end(), IsSeparator));
			return iter == path.end() ? std::string() : path.substr(0, iter - path.begin());
		}

		// path attributes
		unsigned FileSize(const std::string &path)
		{
			// HACK: limited to MAX_PATH
			HANDLE file = CreateFile(util::win32::Native(path).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
			if (file == INVALID_HANDLE_VALUE) THROW err::PlatformException<err::Win32PlatformTag, typename err::FileNotFoundException<>::Tags>(err::FileNotFoundException<>().What());
			DWORD size = GetFileSize(file, 0);
			if (size == INVALID_FILE_SIZE) THROW err::PlatformException<err::Win32PlatformTag, typename err::FileException<>::Tags>("failed to query file size");
			CloseHandle(file);
			return static_cast<unsigned>(size);
		}
		unsigned ModTime(const std::string &path)
		{
			// HACK: limited to MAX_PATH
			HANDLE file = CreateFile(util::win32::Native(path).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
			if (file == INVALID_HANDLE_VALUE) THROW err::PlatformException<err::Win32PlatformTag, typename err::FileNotFoundException<>::Tags>(err::FileNotFoundException<>().What());
			FILETIME wtime;
			if (!GetFileTime(file, 0, 0, &wtime)) THROW err::PlatformException<err::Win32PlatformTag, typename err::FileException<>::Tags>("failed to query file time");
			CloseHandle(file);
			return static_cast<unsigned>(wtime.dwLowDateTime);
		}

		// directory listing
		void ListDir(const std::string &path, const ListDirCallback &cb)
		{
			// HACK: limited to MAX_PATH
			WIN32_FIND_DATA fd;
			HANDLE findState = FindFirstFile(util::win32::Native(CatPath(path, "*")).c_str(), &fd);
			if (findState == INVALID_HANDLE_VALUE) THROW err::PlatformException<err::Win32PlatformTag, typename err::FileNotFoundException<>::Tags>("invalid directory");
			do {
				std::string file(util::win32::Narrow(util::win32::String(fd.cFileName)));
				if (file != "." && file != "..") cb(file);
			} while (FindNextFile(findState, &fd));
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				FindClose(findState);
				THROW err::PlatformException<err::Win32PlatformTag, typename err::FileException<>::Tags>("failed to list directory");
			}
			FindClose(findState);
		}
		void WalkDir(const std::string &path, const ListDirCallback &cb, const WalkDirRecurseFilter &recurse)
		{
			ListDir(path, std::bind(WalkNextDir, path, std::string(), std::placeholders::_1, cb, recurse));
		}
	}
}
