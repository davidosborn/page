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
#include <utility> // pair

namespace page
{
	namespace util
	{
		std::string GetExtension(const std::string &path, unsigned index)
		{
			std::string::size_type start, end(path.npos);
			for (;;)
			{
				start = path.rfind('.', end);
				if (!index || start == path.npos) break;
				end = start;
				--index;
			}
			return start != path.npos ? path.substr(start + 1, end) : "";
		}

		std::pair<std::string, std::string>
			PartitionExtension(const std::string &path)
		{
			auto i(path.rfind('.'));
			return i != path.npos ?
				{path.substr(0, i), path.substr(i + 1)} :
				{path, ""};
		}

		std::string AddExtension(
			std::string        path,
			std::string const& extension)
		{
			if (path.empty() || path.back() != '.')
				path += '.';
			return path += extension;
		}
	}
}
