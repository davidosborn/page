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

			/*-------------+
			| constructors |
			+-------------*/

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

			/*-------------+
			| data members |
			+-------------*/

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
