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
