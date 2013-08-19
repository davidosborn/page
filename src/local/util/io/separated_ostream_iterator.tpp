#include <ostream> // basic_ostream

namespace page
{
	namespace util
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename T, typename Char, typename CharTraits>
		template <typename Separator>
			separated_ostream_iterator<T, Char, CharTraits>::
			separated_ostream_iterator(Stream &os, Separator separator) :
				os(os), separator(separator) {}

		/*--------------------------------+
		| std::ostream_iterator semantics |
		+--------------------------------*/

		template <typename T, typename Char, typename CharTraits>
			separated_ostream_iterator<T, Char, CharTraits> &
			separated_ostream_iterator<T, Char, CharTraits>::
			operator =(const T &value)
		{
			if (!atFirstElement) os << separator;
			os << value;
			atFirstElement = false;
			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_ostream_iterator<T, Char, CharTraits> &
			separated_ostream_iterator<T, Char, CharTraits>::
			operator *()
		{
			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_ostream_iterator<T, Char, CharTraits> &
			separated_ostream_iterator<T, Char, CharTraits>::
			operator ++()
		{
			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_ostream_iterator<T, Char, CharTraits> &
			separated_ostream_iterator<T, Char, CharTraits>::
			operator ++(int)
		{
			return *this;
		}
	}
}
