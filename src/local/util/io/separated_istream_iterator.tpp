#include <cassert> // assert
#include <istream> // basic_istream
#include <string> // basic_string

namespace page
{
	namespace util
	{
		/*-------------+
		| constructors |
		+-------------*/

		template <typename T, typename Char, typename CharTraits>
			separated_istream_iterator<T, Char, CharTraits>::
			separated_istream_iterator() {}

		template <typename T, typename Char, typename CharTraits>
		template <typename Separator>
			separated_istream_iterator<T, Char, CharTraits>::
			separated_istream_iterator(Stream &is, Separator separator) :
				is(&is), separator(separator)
		{
			is >> value;
		}

		/*--------------------------------+
		| std::istream_iterator semantics |
		+--------------------------------*/

		template <typename T, typename Char, typename CharTraits>
			const T &
			separated_istream_iterator<T, Char, CharTraits>::
			operator *() const
		{
			return value;
		}

		template <typename T, typename Char, typename CharTraits>
			const T *
			separated_istream_iterator<T, Char, CharTraits>::
			operator ->() const
		{
			return &value;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_istream_iterator<T, Char, CharTraits> &
			separated_istream_iterator<T, Char, CharTraits>::
			operator ++()
		{
			assert(is);

			if (separator)
			{
				Char c;
				*is >> c;
				if (!separator(c))
					is->setstate(std::ios_base::failbit);
			}

			*is >> value;
			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_istream_iterator<T, Char, CharTraits>
			separated_istream_iterator<T, Char, CharTraits>::
			operator ++(int)
		{
			separated_istream_iterator tmp(*this);
			++*this;
			return tmp;
		}

		/*---------------------+
		| relational operators |
		+---------------------*/

		template <typename T, typename Char, typename CharTraits>
			bool separated_istream_iterator<T, Char, CharTraits>::operator ==(const separated_istream_iterator<T, Char, CharTraits> &other) const
		{
			return (is && *is) == (other.is && *other.is);
		}

		template <typename T, typename Char, typename CharTraits>
			bool separated_istream_iterator<T, Char, CharTraits>::operator !=(const separated_istream_iterator<T, Char, CharTraits> &other) const
		{
			return !(*this == other);
		}
	}
}
