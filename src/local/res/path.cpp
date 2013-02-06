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
 *
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

#include <algorithm> // replace
#include <string>

namespace page
{
	namespace res
	{
		std::string NormPath(const std::string &path)
		{
			std::string normPath(path);
			// replace backslashes with forward slashes
			std::replace(normPath.begin(), normPath.end(), '\\', '/');
			// remove leading and trailing slashes
			normPath.erase(0, normPath.find_first_not_of('/'));
			normPath.erase(normPath.find_last_not_of('/') + 1);
			// merge adjacent slashes
			for (std::string::size_type sep(std::string::npos);
				(sep = normPath.find("//", sep + 2)) != std::string::npos;)
				normPath.erase(sep + 1, normPath.find_first_not_of('/', sep + 2) - sep - 1);
			return normPath;
		}
		std::string CatPath(const std::string &path1, const std::string &path2)
		{
			if (path1.empty()) return path2;
			if (path2.empty()) return path1;
			return path1 + '/' + path2;
		}

		std::string BaseName(const std::string &path)
		{
			return path.substr(path.rfind('/') + 1);
		}
		std::string DirName(const std::string &path)
		{
			std::string::size_type sep = path.rfind('/');
			return sep != std::string::npos ? path.substr(0, sep) : "";
		}
	}
}
