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

			/*-------------+
			| constructors |
			+-------------*/

			public:
			/**
			 * Creates an output iterator with a given separator.
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

			/*-------------+
			| data members |
			+-------------*/

			private:
			Stream &os;
			Delimiter separator;
			bool atFirstElement = true;
		};
	}
}

#	include "separated_ostream_iterator.tpp"
#endif
