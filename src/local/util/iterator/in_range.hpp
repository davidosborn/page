#ifndef    page_util_iterator_in_range_hpp
#   define page_util_iterator_in_range_hpp

#	include "range.hpp"

namespace page { namespace util
{
	/**
	 * Returns @c true if the iterator points to an element within the range.
	 */
	template <typename Iterator>
		bool in_range(
			const range<Iterator> &range,
			const typename decltype(range)::const_iterator &);
}}

#endif
