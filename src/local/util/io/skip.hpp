#ifndef    page_local_util_io_skip_hpp
#   define page_local_util_io_skip_hpp

#	include <iosfwd> // basic_istream
#	include <limits> // numeric_limits
#	include <utility> // pair

#	include "InputDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Skips over a run of delimiters.
		 *
		 * @return A @c std::pair containing @c true if the stream has not
		 *         reached either EOF or a terminating character and @c failbit
		 *         is not set, and the number of delimiters that were skipped.
		 */
		template <typename Char, typename CharTraits,
			typename Separator  = const InputDelimiter<Char, CharTraits> &,
			typename Terminator = const InputDelimiter<Char, CharTraits> &>
			std::pair<bool, unsigned> Skip(
				std::basic_istream<Char, CharTraits> &,
				Separator  = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				Terminator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				unsigned limit = std::numeric_limits<unsigned>::max());
	}
}

#	include "skip.tpp"
#endif
