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

#include <cerrno>
#include <climits> // UINT_MAX
#include <dirent.h> // {close,open,read}dir
#include <libgen.h> // {base,dir}name
#include <sys/stat.h> // stat
#include "../../err/exception/throw.hpp" // THROW
#include "../../err/posix.hpp" // {Check,Reset}Error
#include "../file.hpp" // ListDirCallback

namespace page
{
	namespace sys
	{
		namespace
		{
			// directory recursion
			void WalkNextDir(const std::string &root, const std::string &base, const std::string &name, const ListDirCallback &cb)
			{
				std::string path(CatPath(base, name));
				std::string absPath(CatPath(root, path));
				if (IsDir(absPath))
					ListDir(absPath, std::bind(WalkNextDir, root, path, std::placeholders::_1, cb));
				else cb(path);
			}
		}

		// character types
		bool IsSeparator(char c)
		{
			return c == '/';
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
			struct stat st;
			if (stat(path.c_str(), &st)) switch (errno)
			{
				case ENOENT:
				case ENOTDIR: return false;
				default: err::posix::CheckError();
			}
			return true;
		}
		bool IsFile(const std::string &path)
		{
			struct stat st;
			if (stat(path.c_str(), &st)) err::posix::CheckError();
			return S_ISREG(st.st_mode);
		}
		bool IsDir(const std::string &path)
		{
			struct stat st;
			if (stat(path.c_str(), &st)) err::posix::CheckError();
			return S_ISDIR(st.st_mode);
		}
		bool IsAbsPath(const std::string &path)
		{
			return IsSeparator(path[0]);
		}

		// path conversion
		std::string NormPath(const std::string &path)
		{
			if (path.empty()) return path;
			std::string normPath(path);
			// collapse separators
			// FIXME: implement
			// remove extraneous hierarchy descents
			// FIXME: implement
			// make relative paths explicit
			if (!IsSeparator(normPath[0]) && (normPath[0] != '.' || !IsSeparator(normPath[1])))
				normPath.insert(0, "./");
			return normPath;
		}
		std::string CatPath(const std::string &path1, const std::string &path2)
		{
			if (path1.empty()) return path2;
			if (path2.empty()) return path1;
			return path1 + '/' + path2;
		}

		// path extraction
		std::string BaseName(const std::string &path)
		{
			std::string mutablePath(path);
			return basename(const_cast<char *>(mutablePath.c_str()));
		}
		std::string DirName(const std::string &path)
		{
			std::string mutablePath(path);
			return dirname(const_cast<char *>(mutablePath.c_str()));
		}

		// path attributes
		unsigned FileSize(const std::string &path)
		{
			struct stat st;
			if (stat(path.c_str(), &st)) err::posix::CheckError();
			if (!S_ISREG(st.st_mode)) THROW err::PlatformException<err::PosixPlatformTag, typename err::FileException<>::Tags>("file has no size");
			return st.st_size;
		}
		unsigned ModTime(const std::string &path)
		{
			struct stat st;
			if (stat(path.c_str(), &st)) err::posix::CheckError();
			return st.st_mtime % UINT_MAX;
		}

		// directory listing
		void ListDir(const std::string &path, const ListDirCallback &cb)
		{
			DIR *dir = opendir(path.c_str());
			if (!dir) switch (errno)
			{
				case ENOENT:
				case ENOTDIR: THROW err::PlatformException<err::PosixPlatformTag, typename err::NotDirectoryException<>::Tags>(err::NotDirectoryException<>().What());
				default: err::posix::CheckError();
			}
			try
			{
				err::posix::ResetError();
				for (dirent *entry; entry = readdir(dir);)
				{
					std::string name(entry->d_name);
					if (name != "." && name != "..") cb(name);
				}
				err::posix::CheckError();
			}
			catch (...)
			{
				closedir(dir);
				throw;
			}
			closedir(dir);
		}
		void WalkDir(const std::string &path, const ListDirCallback &cb)
		{
			ListDir(path, std::bind(WalkNextDir, path, std::string(), std::placeholders::_1, cb));
		}
	}
}
