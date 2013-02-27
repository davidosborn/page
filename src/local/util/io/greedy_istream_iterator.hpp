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

#ifndef    page_local_util_io_greedy_istream_iterator_hpp
#   define page_local_util_io_greedy_istream_iterator_hpp

#	include <iosfwd> // basic_istream
#	include <iterator> // input_iterator_tag, iterator, ptrdiff_t

#	include "InputDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * An enhanced alternative to @c std::istream_iterator, which supports
		 * greedy sequence deserialization.
		 *
		 * This class has the following behaviour:
		 * - You may specify a terminator, which will transform the iterator
		 *   into an end-of-stream iterator when it is encountered.
		 * - This class will read everything until the next delimiter or the
		 *   end-of-stream, and then attempt to deserialize the value from what
		 *   was read.
		 * - When no characters are available before the next delimiter, the
		 *   value is default initialized.
		 */
		template <
			typename T,
			typename Char       = char,
			typename CharTraits = std::char_traits<Char>>

			class greedy_istream_iterator :
				public std::iterator<std::input_iterator_tag, T, std::ptrdiff_t, const T *, const T &>
		{
			private:
			typedef std::basic_istream<Char, CharTraits> Stream;
			typedef InputDelimiter<Char, CharTraits>     Delimiter;

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			/**
			 * Creates an end-of-stream iterator.
			 */
			greedy_istream_iterator();

			/**
			 * Creates an input-stream iterator with a given separator and
			 * terminator.
			 */
			template <
				typename Separator  = const Delimiter &,
				typename Terminator = const Delimiter &>
				explicit greedy_istream_iterator(Stream &,
					Separator  = Delimiter::GetSpaceDelimiter(),
					Terminator = Delimiter::GetEmptyDelimiter());

			/*--------------------------------+
			| std::istream_iterator semantics |
			+--------------------------------*/

			const T &operator *() const;
			const T *operator ->() const;
			greedy_istream_iterator &operator ++();
			greedy_istream_iterator operator ++(int);

			/*---------------------+
			| relational operators |
			+---------------------*/

			bool operator ==(const greedy_istream_iterator &) const;
			bool operator !=(const greedy_istream_iterator &) const;

			/*-----------------+
			| member variables |
			+-----------------*/

			private:
			Stream *is = nullptr;
			Delimiter separator, terminator;
			T value;

			/**
			 * Indicates that this iterator will become an end-of-stream
			 * iterator the next time it advances.
			 */
			bool terminated = false;
		};
	}
}

#	include "greedy_istream_iterator.tpp"
#endif
