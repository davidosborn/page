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

#include "../iterator.hpp" // in_range

namespace page
{
	namespace util
	{
		// constructors
		template <typename Iterator>
			chain_iterator<Iterator>::chain_iterator(Iterator iter) :
			chain_iterator(std::initializer_list<range<Iterator>>(), iter) {}
		template <typename Iterator>
			chain_iterator<Iterator>::chain_iterator(std::initializer_list<std::pair<Iterator, Iterator>> ranges, Iterator tail) :
			Base(tail), tail(tail), currentRange(this->ranges.begin())
		{
			this->ranges.reserve(ranges.size());
			for (const auto &range : ranges)
				this->ranges.push_back(make_range(range));
			currentRange = this->ranges.begin();
			
			Init();
		}
		template <typename Iterator>
			chain_iterator<Iterator>::chain_iterator(std::initializer_list<range<Iterator>> ranges, Iterator tail) :
			Base(tail), ranges(ranges), tail(tail), currentRange(this->ranges.begin())
		{
			Init();
		}
		template <typename Iterator> template <typename Iterator2>
			chain_iterator<Iterator>::chain_iterator(const chain_iterator<Iterator2> &other) :
			Base(other.iter), ranges(other.ranges), tail(other.tail),
			currentRange(ranges.begin() + (other.currentRange - other.ranges.begin())) {}

		// initialization
		template <typename Iterator> void chain_iterator<Iterator>::Init()
		{
			if (currentRange != ranges.end())
			{
				iter = currentRange->begin();
				while (iter == currentRange->end())
					if (++currentRange == ranges.end())
					{
						iter = tail;
						break;
					}
					else iter = currentRange->begin();
			}
		}
		
		// arithmetic
		template <typename Iterator>
			void chain_iterator<Iterator>::Increment()
		{
			Base::Increment();
			if (currentRange != ranges.end())
				while (iter == currentRange->end())
					if (++currentRange == ranges.end())
					{
						iter = tail;
						break;
					}
					else iter = currentRange->begin();
		}
		template <typename Iterator>
			void chain_iterator<Iterator>::Decrement()
		{
			if (currentRange == ranges.end())
			{
				if (iter == tail)
				{
					assert(currentRange != ranges.begin());
					iter = (--currentRange)->end();
					goto inRange;
				}
			}
			else
			{
				inRange:
				while (iter == currentRange->begin())
				{
					assert(currentRange != ranges.begin());
					iter = (--currentRange)->end();
				}
			}
			Base::Decrement();
		}
		template <typename Iterator>
			void chain_iterator<Iterator>::Advance(difference_type n)
		{
			if (n > 0)
			{
				if (currentRange != ranges->end())
				{
					do
					{
						difference_type remainder = currentRange->end() - iter;
						if (remainder <= n)
						{
							n -= remainder;
							if (++currentRange == ranges.end())
							{
								iter = tail + n;
								return;
							}
							iter = currentRange->begin();
						}
						else
						{
							Base::Advance(n);
							return;
						}
					}
					while (n > 0);
				}
				else Base::Advance(n);
			}
			else if (n < 0)
			{
				if (currentRange == ranges->end())
				{
					difference_type remainder = tail - iter;
					if (remainder <= n)
					{
						Base::Advance(n);
						return;
					}
					else
					{
						assert(currentRange != ranges.begin());
						iter = (--currentRange)->end();
						n -= remainder;
					}
				}
				do
				{
					difference_type remainder = currentRange->begin() - iter;
					if (remainder <= n)
					{
						Base::Advance(n);
						return;
					}
					else
					{
						assert(currentRange != ranges.begin());
						iter = (--currentRange)->end();
						n -= remainder;
					}
				}
				while (n < 0);
			}
		}

		// distance
		template <typename Iterator>
			typename chain_iterator<Iterator>::difference_type
			chain_iterator<Iterator>::Distance(const chain_iterator &other) const
		{
			typename Ranges::const_iterator otherRange(WhichRange(other.iter));
			if (otherRange == currentRange) return other.iter - iter;
			if (otherRange == ranges.end()) return other.iter - tail;
			if (otherRange < currentRange)
			{
				difference_type n = currentRange->begin() - iter;
				for (const auto &range(currentRange); range != otherRange; --range)
					n += range->begin() - range->end();
				n += other.iter - otherRange->end();
				return n;
			}
			// (otherRange > currentRange && otherRange < ranges.end())
			{
				difference_type n = currentRange->end() - iter;
				for (const auto &range(currentRange); range != otherRange; ++range)
					n += range->end() - range->begin();
				n += other.iter - otherRange.begin();
				return n;
			}
		}

		// comparison
		template <typename Iterator>
			bool chain_iterator<Iterator>::Less(const chain_iterator &other) const
		{
			if (!ranges.empty())
			{
				if (!other.ranges.empty())
				{
					assert(ranges == other.ranges && tail == other.tail);
					auto rdiff = (currentRange - ranges.begin()) - (other.currentRange - other.ranges.begin());
					return rdiff == 0 ? iter < other.iter : rdiff < 0;
				}
				// else
				{
					typename Ranges::const_iterator otherRange(WhichRange(other.iter));
					return
						otherRange == currentRange ? iter < other.iter :
						otherRange <  currentRange ? false : true;
				}
			}
			else
			{
				assert(!other.ranges.empty());
				return other > *this;
			}
		}
		
		// mapping
		template <typename Iterator>
			typename chain_iterator<Iterator>::Ranges::const_iterator
			chain_iterator<Iterator>::WhichRange(const Iterator &iter) const
		{
			auto range(ranges.begin());
			for (; range != ranges.end(); ++range)
				if (in(*range, iter)) break;
			return range;
		}

		// factory functions
		template <typename Iterator>
			chain_iterator<Iterator> make_chain_iterator(Iterator iter)
		{
			return chain_iterator<Iterator>(iter);
		}
		template <typename Iterator>
			chain_iterator<Iterator> make_chain_iterator(std::initializer_list<std::pair<Iterator, Iterator>> il, Iterator tail)
		{
			return chain_iterator<Iterator>(il, tail);
		}
		template <typename Iterator>
			chain_iterator<Iterator> make_chain_iterator(std::initializer_list<range<Iterator>> il, Iterator tail)
		{
			return chain_iterator<Iterator>(il, tail);
		}
	}
}
