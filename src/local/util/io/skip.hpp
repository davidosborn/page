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

#ifndef    page_local_util_io_skip_hpp
#   define page_local_util_io_skip_hpp

#	include <iosfwd> // basic_istream
#	include <limits> // numeric_limits
#	include <utility> // pair

#	include "InputDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Skips over a run of delimiters.
		 *
		 * @return A @c std::pair containing @c true if the stream has not
		 *         reached either EOF or a terminating character and @c failbit
		 *         is not set, and the number of delimiters that were skipped.
		 */
		template <typename Char, typename CharTraits,
			typename Separator  = const InputDelimiter<Char, CharTraits> &,
			typename Terminator = const InputDelimiter<Char, CharTraits> &>
			std::pair<bool, unsigned> Skip(
				std::basic_istream<Char, CharTraits> &,
				Separator  = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				Terminator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				unsigned limit = std::numeric_limits<unsigned>::max());
	}
}

#	include "skip.tpp"
#endif
