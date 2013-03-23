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

namespace page
{
	namespace util
	{
		// constructors
		template <typename Iterator, int step>
			step_iterator<Iterator, step>::step_iterator() {}
		template <typename Iterator, int step>
			step_iterator<Iterator, step>::step_iterator(const Iterator &iter) :
			Base(iter) {}
		template <typename Iterator, int step> template <typename Iterator2>
			step_iterator<Iterator, step>::step_iterator(const step_iterator<Iterator2, step> &other) :
			Base(other.iter) {}

		// arithmetic
		template <typename Iterator, int step>
			void step_iterator<Iterator, step>::Increment()
		{
			Base::Advance(step);
		}
		template <typename Iterator, int step>
			void step_iterator<Iterator, step>::Decrement()
		{
			Base::Advance(-step);
		}
		template <typename Iterator, int step>
			void step_iterator<Iterator, step>::Advance(difference_type n)
		{
			Base::Advance(n * step);
		}

		// distance
		template <typename Iterator, int step>
			typename step_iterator<Iterator, step>::difference_type
			step_iterator<Iterator, step>::Distance(const step_iterator &other) const
		{
			return Base::Distance(other) / step;
		}

		// reverse arithmetic
		template <typename Iterator, int step>
			step_iterator<Iterator, step> operator +(
				typename step_iterator<Iterator, step>::difference_type n,
				const step_iterator<Iterator, step> &iter)
		{
			return iter + n;
		}
	}
}
