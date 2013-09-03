#ifndef    page_local_util_io_separated_istream_iterator_hpp
#   define page_local_util_io_separated_istream_iterator_hpp

#	include <iosfwd> // basic_istream
#	include <iterator> // input_iterator_tag, iterator, ptrdiff_t

#	include "InputDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * An enhanced alternative to @c std::istream_iterator, which supports
		 * reading a separator between elements.
		 */
		template <
			typename T,
			typename Char       = char,
			typename CharTraits = std::char_traits<Char>>

			class separated_istream_iterator :
				public std::iterator<std::input_iterator_tag, T, std::ptrdiff_t, const T *, const T &>
		{
			private:
			typedef std::basic_istream<Char, CharTraits> Stream;
			typedef InputDelimiter<Char, CharTraits>     Delimiter;

			/*-------------+
			| constructors |
			+-------------*/

			public:
			/**
			 * Creates an end-of-stream iterator.
			 */
			separated_istream_iterator();

			/**
			 * Creates an input iterator with the specified separator.
			 */
			template <typename Separator = const Delimiter &>
				explicit separated_istream_iterator(Stream &,
					Separator = Delimiter::GetSpaceDelimiter());

			/*--------------------------------+
			| std::istream_iterator semantics |
			+--------------------------------*/

			public:
			const T &operator *() const;
			const T *operator ->() const;
			separated_istream_iterator &operator ++();
			separated_istream_iterator operator ++(int);

			/*---------------------+
			| relational operators |
			+---------------------*/

			public:
			bool operator ==(const separated_istream_iterator &) const;
			bool operator !=(const separated_istream_iterator &) const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			Stream *is = nullptr;
			Delimiter separator;
			T value;
		};
	}
}

#	include "separated_istream_iterator.tpp"
#endif
