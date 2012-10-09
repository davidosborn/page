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

#ifndef    page_local_sys_file_hpp
#   define page_local_sys_file_hpp

#	include <functional> // function
#	include <string>

namespace page
{
	namespace sys
	{
		// character types
		bool IsSeparator(char);
		bool IsWildcard(char);
		bool IsReserved(char);
		bool IsSpecial(char);
		bool IsInvalid(char);

		// path validation
		bool PathExists(const std::string &);
		bool IsFile(const std::string &);
		bool IsDir(const std::string &);
		bool IsAbsPath(const std::string &);

		// path conversion
		std::string AbsPath(const std::string &);
		std::string NormPath(const std::string &);
		std::string CatPath(const std::string &, const std::string &);

		// path extraction
		std::string BaseName(const std::string &);
		std::string DirName(const std::string &);

		// path attributes
		unsigned FileSize(const std::string &);
		unsigned ModTime(const std::string &);

		// directory listing
		typedef std::function<void (const std::string &)> ListDirCallback;
		typedef std::function<bool (const std::string &)> WalkDirRecurseFilter;
		void ListDir(const std::string &, const ListDirCallback &);
		void WalkDir(const std::string &, const ListDirCallback &, const WalkDirRecurseFilter & = 0);
	}
}

#endif
