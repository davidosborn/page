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

#ifndef    page_local_util_io_separated_ostream_iterator_hpp
#   define page_local_util_io_separated_ostream_iterator_hpp

#	include <iosfwd> // basic_ostream
#	include <iterator> // iterator, output_iterator_tag

#	include "OutputDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * An enhanced alternative to @c std::ostream_iterator, which supports
		 * writing a separator between elements.
		 */
		template <
			typename T,
			typename Char       = char,
			typename CharTraits = std::char_traits<Char>>

			class separated_ostream_iterator :
				public std::iterator<std::output_iterator_tag, void, void, void, void>
		{
			private:
			typedef std::basic_ostream<Char, CharTraits> Stream;
			typedef OutputDelimiter<Char, CharTraits>    Delimiter;

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			/**
			 * Creates an output-stream iterator with a given separator.
			 */
			template <typename Separator = const Delimiter &>
				explicit separated_ostream_iterator(Stream &,
					Separator = Delimiter::GetSpaceDelimiter());

			/*--------------------------------+
			| std::ostream_iterator semantics |
			+--------------------------------*/

			public:
			separated_ostream_iterator &operator =(const T &);
			separated_ostream_iterator &operator *();
			separated_ostream_iterator &operator ++();
			separated_ostream_iterator &operator ++(int);

			/*-----------------+
			| member variables |
			+-----------------*/

			private:
			Stream &os;
			Delimiter separator;
			bool atFirstElement = true;
		};
	}
}

#	include "separated_ostream_iterator.tpp"
#endif
