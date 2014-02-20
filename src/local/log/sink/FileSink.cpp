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

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../util/path/filesystem.hpp" // AbsolutePath
#include "FileSink.hpp"

namespace page
{
	namespace log
	{
		/*-------------+
		| constructors |
		+-------------*/

		FileSink::FileSink(std::string path) :
			fs(util::AbsolutePath(path))
		{
			if (!fs)
				THROW((err::Exception<err::LogModuleTag, err::FileTag>("failed to open file stream") <<
					boost::errinfo_api_function("std::ofstream::open") <<
					boost::errinfo_file_name(path) <<
					boost::errinfo_file_open_mode("w")))
		}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void FileSink::DoWrite(const std::string &s)
		{
			if (!fs.write(s.data(), s.size()))
				THROW((err::Exception<err::LogModuleTag, err::FileWriteTag>()))
		}
	}
}
