/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
