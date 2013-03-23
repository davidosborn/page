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
		template <typename Value>
			repeat_iterator<Value>::repeat_iterator() : count(0) {}
		template <typename Value>
			repeat_iterator<Value>::repeat_iterator(const Value &value, difference_type count) :
			value(value), count(count) {}
		template <typename Value> template <typename Value2>
			repeat_iterator<Value>::repeat_iterator(const repeat_iterator<Value2> &other) :
			value(other.value), count(other.count) {}

		// dereference
		template <typename Value>
			typename repeat_iterator<Value>::reference
			repeat_iterator<Value>::Dereference() const
		{
			return value;
		}

		// arithmetic
		template <typename Value>
			void repeat_iterator<Value>::Increment()
		{
			--count;
		}
		template <typename Value>
			void repeat_iterator<Value>::Decrement()
		{
			++count;
		}
		template <typename Value>
			void repeat_iterator<Value>::Advance(difference_type n)
		{
			count -= n;
		}

		// distance
		template <typename Value>
			typename repeat_iterator<Value>::difference_type
			repeat_iterator<Value>::Distance(const repeat_iterator &other) const
		{
			return count - other.count;
		}

		// comparison
		template <typename Value>
			bool repeat_iterator<Value>::Equal(const repeat_iterator &other) const
		{
			return count == other.count;
		}
		template <typename Value>
			bool repeat_iterator<Value>::Less(const repeat_iterator &other) const
		{
			return count > other.count;
		}
	}
}
