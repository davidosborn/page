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

// stream line iterator
// counts lines and tracks indentation
// points to empty string at end of stream

#ifndef    page_local_res_adapt_text_LineIterator_hpp
#   define page_local_res_adapt_text_LineIterator_hpp

#	include <string>

namespace page
{
	namespace res
	{
		class Stream;

		struct LineIterator
		{
			typedef std::string value_type;
			typedef const value_type *pointer;
			typedef const value_type &reference;

			LineIterator(Stream &, char comment = '#');

			reference operator *() const;
			pointer operator ->() const;

			LineIterator &operator ++();
			LineIterator operator ++(int);

			explicit operator bool() const;

			unsigned Indentation() const;

			private:
			void Fetch();
			void Fetch() const;

			Stream &stream;
			char comment;
			std::string line, space;
			unsigned indentation;
			bool warned;
		};
	}
}

#endif
