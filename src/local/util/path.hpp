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

#ifndef    page_local_util_path_hpp
#   define page_local_util_path_hpp

	// C++
#	include <utility> // pair

	// Boost
#	include <boost/filesystem/path.hpp>

namespace page
{
	namespace util
	{
		/*------------------+
		| path manipulation |
		+------------------*/

		/**
		 * Parse and expand the special symbols in a path.
		 *
		 * Supports strftime and incremental symbols.  Incremental symbols are
		 * denoted by %i with an optional width modifier after the percent sign.
		 * The width modifier must be an unsigned decimal integer.  Multiple
		 * incremental symbols are permitted; they will form an overflow
		 * cascade.  If no unique incremental filename is found, an err::Range
		 * is thrown.  Null characters will be thrown away.
		 */
		boost::filesystem::path ExpandPath(const boost::filesystem::path &);

		/*-----------------------+
		| extension manipulation |
		+-----------------------*/

		/**
		 * Return the path's extension, without the leading dot character.  If
		 * the path has multiple extensions, return the one at the specified
		 * index, counting back from the right.  The extension is converted to
		 * lowercase.
		 */
		boost::filesystem::path GetExtension(
			const boost::filesystem::path &path,
			unsigned index = 0);

		/**
		 * Return the two path segments resulting from splitting the path at the
		 * last dot character.
		 */
		std::pair<boost::filesystem::path, boost::filesystem::path>
			PartitionExtension(const boost::filesystem::path &path);

		/**
		 * Add the extension to the end of the path.  If the path does not end
		 * with a dot character, add one before the extension.
		 */
		boost::filesystem::path AddExtension(
			const boost::filesystem::path &path,
			const boost::filesystem::path &extension);

		/**
		 * Ensure the path ends with the specified extension.
		 */
		boost::filesystem::path WithExtension(
			const boost::filesystem::path &path,
			const boost::filesystem::path &extension);

		/**
		 * Ensure the path ends with one of the specified extensions.  If it
		 * does not, the first extension is used.
		 */
		template <typename InputIterator>
			boost::filesystem::path WithExtension(
				const boost::filesystem::path &path,
				InputIterator firstExtension,
				InputIterator lastExtension);
	}
}

#	include "path.tpp"
#endif
