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

#include <istream>

namespace page
{
	namespace util
	{
		template <typename Char, typename CharTraits,
			typename Separator, typename Terminator>
			std::pair<bool, unsigned> Skip(
				std::basic_istream<Char, CharTraits> &is,
				Separator separatorArg, Terminator terminatorArg,
				unsigned limit)
		{
			InputDelimiter<Char, CharTraits>
				separator(separatorArg), terminator(terminatorArg);

			/**
			 * @hack If we don't do this, and EOF has already been reached,
			 *       @c std::basic_istream::peek will set @c failbit.
			 */
			if (!is.good())
				return std::make_pair(false, 0);

			unsigned n = 0;
			for (; n < limit; ++n)
			{
				auto c = is.peek();
				if (!CharTraits::not_eof(c))
					return std::make_pair(false, n);
				auto ch = CharTraits::to_char_type(c);
				if (terminator(ch))
				{
					is.ignore(); // skip terminator
					return std::make_pair(false, n);
				}
				if (!separator(ch)) break;
				is.ignore();
			}
			return std::make_pair(!is.fail(), n);
		}
	}
}
