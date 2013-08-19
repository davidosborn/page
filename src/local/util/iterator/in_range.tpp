#include <algorithm> // find
#include <iterator> // iterator_traits, random_access_iterator_tag
#include <type_traits> // is_same

#include "../type_traits/sfinae.hpp" // ENABLE_IF
#include "deferred_iterator.hpp"

namespace page { namespace util
{
	namespace detail
	{
		template <typename Iterator>
			bool in_range_impl(
				const range<Iterator> &range,
				const typename decltype(range)::const_iterator &iter,
				ENABLE_IF((std::is_same<
					typename std::iterator_traits<decltype(iter)>::iterator_category,
					std::random_access_iterator_tag>::value)))
		{
			return iter >= std::begin(range) && iter < std::end(range);
		}

		template <typename Iterator>
			bool in_range_impl(
				const range<Iterator> &range,
				const typename decltype(range)::const_iterator &iter,
				ENABLE_IF((!std::is_same<
					typename std::iterator_traits<decltype(iter)>::iterator_category,
					std::random_access_iterator_tag>::value)))
		{
			return std::find(
				make_deferred_iterator(std::begin(range)),
				make_deferred_iterator(std::end(range)),
				iter).base() != range.end();
		}
	}

	template <typename Iterator>
		bool in_range(
			const range<Iterator> &range,
			const typename decltype(range)::const_iterator &iter)
	{
		return detail::in_range_impl(range, iter);
	}
}}
