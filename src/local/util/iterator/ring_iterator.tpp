/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

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

#include <cassert>

namespace page
{
	namespace util
	{
		// constructors
		template <typename Iterator>
			ring_iterator<Iterator>::ring_iterator(Iterator first, Iterator last) :
			ring_iterator(make_range(first, last)) {}
		template <typename Iterator>
			ring_iterator<Iterator>::ring_iterator(Iterator first, Iterator last, Iterator current) :
			ring_iterator(make_range(first, last), current) {}
		template <typename Iterator>
			ring_iterator<Iterator>::ring_iterator(const util::range<Iterator> &range) :
			Base(range.begin()), range(range) {}
		template <typename Iterator>
			ring_iterator<Iterator>::ring_iterator(const util::range<Iterator> &range, Iterator current) :
			Base(current), range(range)
		{
			assert(in(range, current));
		}
		template <typename Iterator> template <typename Iterator2>
			ring_iterator<Iterator>::ring_iterator(const ring_iterator<Iterator2> &other) :
			Base(other.iter) {}

		// arithmetic
		template <typename Iterator>
			void ring_iterator<Iterator>::Increment()
		{
			assert(range.begin() != range.end());
			Base::Increment();
			if (Base::iter == range.end())
				Base::iter = range.begin();
		}
		template <typename Iterator>
			void ring_iterator<Iterator>::Decrement()
		{
			assert(range.begin() != range.end());
			if (Base::iter == range.begin())
				Base::iter = range.end();
			Base::Decrement();
		}
		template <typename Iterator>
			void ring_iterator<Iterator>::Advance(difference_type n)
		{
			typename std::iterator_traits<Iterator>::difference_type
				rangeSize        = std::distance(range.begin(), range.end()),
				currentOffset    = std::distance(range.begin(), Base::iter),
				currentRemainder = rangeSize - currentOffset;
			n = n % rangeSize; // avoid redundant laps around the ring
			if      (n > currentRemainder) Base::iter = range.begin() + n - currentRemainder;
			else if (n < -currentOffset)   Base::iter = range.end()   + n + currentOffset - 1;
			else Base::Advance(n);
		}

		// factory functions
		template <typename Iterator>
			ring_iterator<Iterator> make_ring_iterator(Iterator first, Iterator last)
		{
			return ring_iterator<Iterator>(first, last);
		}
		template <typename Iterator>
			ring_iterator<Iterator> make_ring_iterator(Iterator first, Iterator last, Iterator current)
		{
			return ring_iterator<Iterator>(first, last, current);
		}
		template <typename Iterator>
			ring_iterator<Iterator> make_ring_iterator(const util::range<Iterator> &range)
		{
			return ring_iterator<Iterator>(range);
		}
		template <typename Iterator>
			ring_iterator<Iterator> make_ring_iterator(const util::range<Iterator> &range, Iterator current)
		{
			return ring_iterator<Iterator>(range, current);
		}
	}
}
