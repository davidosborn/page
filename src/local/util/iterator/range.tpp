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

#include "deferred_iterator.hpp"

namespace page { namespace util
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename Iterator>
		range<Iterator>::range() :
			range(iterator(), iterator()) {}

	template <typename Iterator>
		range<Iterator>::range(const iterator &first, const iterator &last) :
			range(std::make_pair(first, last)) {}

	template <typename Iterator>
		range<Iterator>::range(const std::pair<iterator, iterator> &iter) :
			iter(iter) {}

	template <typename Iterator> template <typename Range>
		range<Iterator>::range(const Range &other, ENABLE_IF_IMPL(is_range<Range>::value)) :
			range(std::begin(other), std::end(other)) {}

	template <typename Iterator> template <typename Iterator2>
		range<Iterator>::range(const range<Iterator2> &other) :
			range(other.iter) {}

	/*----------+
	| iterators |
	+----------*/

	template <typename Iterator>
		auto range<Iterator>::begin() const noexcept -> const iterator &
	{
		return iter.first;
	}

	template <typename Iterator>
		auto range<Iterator>::end() const noexcept -> const iterator &
	{
		return iter.second;
	}

	template <typename Iterator>
		auto range<Iterator>::rbegin() const -> reverse_iterator
	{
		return reverse_iterator(end());
	}

	template <typename Iterator>
		auto range<Iterator>::rend() const -> reverse_iterator
	{
		return reverse_iterator(begin());
	}

	/*----------+
	| observers |
	+----------*/

	template <typename Iterator>
		range<Iterator>::operator const std::pair<iterator, iterator> &() const noexcept
	{
		return iter;
	}

	template <typename Iterator>
		bool range<Iterator>::empty() const noexcept
	{
		return iter.first == iter.second;
	}

	/*------------------+
	| factory functions |
	+------------------*/

	template <typename Iterator>
		range<Iterator> make_range(Iterator first, Iterator last)
	{
		return range<Iterator>(first, last);
	}

	template <typename Iterator>
		range<Iterator> make_range(const std::pair<Iterator, Iterator> &pair)
	{
		return range<Iterator>(pair);
	}

	template <typename Range,
		typename Iterator = typename range_traits<Range>::iterator>
		range<Iterator> make_range(const Range &other)
	{
		return range<Iterator>(other);
	}
}}
