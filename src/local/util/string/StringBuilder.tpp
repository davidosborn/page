#include <utility> // forward

namespace page
{
	namespace util
	{
		template <typename Char, typename CharTraits>
			template <typename T>
				BasicStringBuilder<Char, CharTraits> &
				BasicStringBuilder<Char, CharTraits>::operator <<(T &&arg)
		{
			ss << std::forward<T>(arg);
			return *this;
		}

		template <typename Char, typename CharTraits>
			BasicStringBuilder<Char, CharTraits>::operator String() const
		{
			return ss.str();
		}
	}
}
