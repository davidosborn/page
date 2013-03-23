/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_util_path_expand_hpp
#   define page_local_util_path_expand_hpp

#	include <string>

#	include "../Flags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A collection of bit-flags for modifying the behaviour of
		 * @c ExpandPath.
		 */
		struct ExpandFlags : Flags
		{
			enum
			{
				/**
				 * Interprets the path as if it had a wildcard suffix.
				 */
				withImplicitWildcardSuffix = 0x01
			};
		};

		/**
		 * Parse and expand the special symbols in a path.
		 *
		 * Supports all of the symbols supported by strftime, as well as the
		 * following extensions:
		 *
		 * %i A number that is incremented each time the file is saved.  The
		 *    function inserts the lowest number that will not conflict with an
		 *    existing filename.  An optional width modifier may be specified
		 *    immediately after the percent sign, which must be an unsigned
		 *    decimal integer.  Multiple incremental symbols form an overflow
		 *    cascade, where the last one increments first.  If no unique
		 *    incremental filename is found, an exception is thrown.
		 *
		 * @note If the path contains wildcards, the function will avoid any
		 *       matching files when processing %i symbols.
		 */
		std::string ExpandPath(const std::string &, int flags = 0);
	}
}

#endif
