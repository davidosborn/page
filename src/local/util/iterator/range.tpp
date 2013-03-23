/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // find
#include <iterator> // iterator_traits, random_access_iterator_tag

#include "deferred_iterator.hpp"

namespace page
{
	namespace util
	{
		namespace detail
		{
			/**
			 * @defgroup range-in-impl Range in-function implementation
			 *
			 * The implementation of @c in.
			 *
			 * @{
			 */
			template <typename Iterator>
				bool in_impl(
					const range<Iterator> &range,
					Iterator               iter,
					ENABLE_IF((std::is_same<
						typename std::iterator_traits<Iterator>::iterator_category,
						std::random_access_iterator_tag>::value)))
			{
				return iter >= range.begin() && iter < range.end();
			}
			template <typename Iterator>
				bool in_impl(
					const range<Iterator> &range,
					Iterator               iter,
					ENABLE_IF((!std::is_same<
						typename std::iterator_traits<Iterator>::iterator_category,
						std::random_access_iterator_tag>::value)))
			{
				return std::find(
					make_deferred_iterator(range.begin()),
					make_deferred_iterator(range.end()), iter).base() != range.end();
			}
			///@}
		}

		// constructors
		template <typename Iterator> range<Iterator>::range() :
			pair(Iterator(), Iterator()) {}
		template <typename Iterator> range<Iterator>::range(Iterator first, Iterator last) :
			pair(first, last) {}
		template <typename Iterator> range<Iterator>::range(const std::pair<Iterator, Iterator> &pair) :
			pair(pair) {}
		template <typename Iterator> template <typename T> range<Iterator>::range(T &other, ENABLE_IF_IMPL((is_range<T>::value))) :
			pair(std::begin(other), std::end(other)) {}
		template <typename Iterator> template <typename T> range<Iterator>::range(const T &other, ENABLE_IF_IMPL((is_range<T>::value))) :
			pair(std::begin(other), std::end(other)) {}
		template <typename Iterator> template <typename Iterator2> range<Iterator>::range(const range<Iterator2> &other) :
			pair(other.pair) {}

		// iterators
		template <typename Iterator> Iterator range<Iterator>::begin() const noexcept
		{
			return pair.first;
		}
		template <typename Iterator> Iterator range<Iterator>::end() const noexcept
		{
			return pair.second;
		}

		// observers
		template <typename Iterator> bool range<Iterator>::empty() const noexcept
		{
			return pair.first == pair.second;
		}

		// conversion
		template <typename Iterator> range<Iterator>::operator const std::pair<Iterator, Iterator> &() const
		{
			return pair;
		}

		// observers
		template <typename Iterator> bool in(const range<Iterator> &range, Iterator iter)
		{
			return detail::in_impl(range, iter);
		}

		// factory functions
		template <typename Iterator> range<Iterator> make_range(Iterator first, Iterator last)
		{
			return range<Iterator>(first, last);
		}
		template <typename Iterator> range<Iterator> make_range(const std::pair<Iterator, Iterator> &pair)
		{
			return range<Iterator>(pair);
		}
		template <typename T> auto make_range(T &other) -> range<decltype(std::begin(std::declval<T &>()))>
		{
			return range<decltype(std::begin(std::declval<T &>()))>(other);
		}
		template <typename T> auto make_range(const T &other) -> range<decltype(std::begin(std::declval<const T &>()))>
		{
			return range<decltype(std::begin(std::declval<const T &>()))>(other);
		}
	}
}
