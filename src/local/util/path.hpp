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

#ifndef    page_util_path_hpp
#   define page_util_path_hpp

#	include <string>
#	include <utility> // pair

namespace page
{
	namespace util
	{
		// symbolic path expansion
		// Parse and expand special symbols in a given path.  Supports strftime
		// and incremental symbols.  Incremental symbols are denoted by %i with
		// an optional width modifier after the percent sign.  The width
		// modifier must be an unsigned decimal integer.  Multiple incremental
		// symbols are permitted; they will form an overflow cascade.  If no
		// unique incremental filename is found, an err::Range is thrown.  Null
		// characters will be thrown away.
		std::string ExpandPath(const std::string &);

		// extension extraction
		typedef std::pair<std::string, std::string> ExtPart;
		ExtPart PartitionExt(const std::string &path);
		std::string GetExt(const std::string &path, unsigned index = 0);

		// extension insertion
		/**
		 * Ensures that the filename pointed to by @c path ends with the given
		 * extension.
		 */
		std::string WithExt(const std::string &path, const std::string &extension);
		/**
		 * Ensures that the filename pointed to by @c path ends with one of the
		 * given extensions.
		 */
		template <typename InputIterator>
			std::string WithExt(const std::string &path, InputIterator firstExtension, InputIterator lastExtension);
	}
}

#	include "path.tpp"
#endif
