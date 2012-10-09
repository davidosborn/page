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

namespace page
{
	namespace util
	{
		// constructors
		template <typename Iterator, typename Function, typename Result>
			call_iterator<Iterator, Function, Result>::call_iterator() {}
		template <typename Iterator, typename Function, typename Result>
			call_iterator<Iterator, Function, Result>::call_iterator(const Iterator &iter, const Function &func) :
			Base(iter), func(func) {}

		// dereference
		template <typename Iterator, typename Function, typename Result>
			typename call_iterator<Iterator, Function, Result>::reference
			call_iterator<Iterator, Function, Result>::Dereference() const
		{
			if (!cache) cache = func(*this->iter);
			return *cache;
		}

		// arithmetic
		template <typename Iterator, typename Function, typename Result>
			void call_iterator<Iterator, Function, Result>::Increment()
		{
			Base::Increment();
			cache = Cache();
		}

		// void result specialization
		// constructors
		template <typename Iterator, typename Function>
			call_iterator<Iterator, Function, void>::call_iterator() {}
		template <typename Iterator, typename Function>
			call_iterator<Iterator, Function, void>::call_iterator(const Iterator &iter, const Function &func) :
			Base(iter), func(func) {}

		// dereference
		template <typename Iterator, typename Function>
			void call_iterator<Iterator, Function, void>::Dereference() const
		{
			if (!cache)
			{
				func(*this->iter);
				cache = true;
			}
		}

		// arithmetic
		template <typename Iterator, typename Function>
			void call_iterator<Iterator, Function, void>::Increment()
		{
			Base::Increment();
			cache = false;
		}
	}
}
