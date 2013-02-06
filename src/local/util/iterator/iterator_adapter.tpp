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
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			iterator_adapter<D, I, C, V, Di, P, R>::iterator_adapter() {}
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			iterator_adapter<D, I, C, V, Di, P, R>::iterator_adapter(const iterator_type &iter) :
			iter(iter) {}

		// base access
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_adapter<D, I, C, V, Di, P, R>::iterator_type
			iterator_adapter<D, I, C, V, Di, P, R>::base() const
		{
			return iter;
		}
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			iterator_adapter<D, I, C, V, Di, P, R>::operator iterator_type() const
		{
			return iter;
		}

		// dereference
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_adapter<D, I, C, V, Di, P, R>::reference
			iterator_adapter<D, I, C, V, Di, P, R>::Dereference() const
		{
			return *iter;
		}

		// arithmetic
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			void iterator_adapter<D, I, C, V, Di, P, R>::Increment()
		{
			++iter;
		}
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			void iterator_adapter<D, I, C, V, Di, P, R>::Decrement()
		{
			--iter;
		}
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			void iterator_adapter<D, I, C, V, Di, P, R>::Advance(difference_type n)
		{
			iter += n;
		}

		// distance
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_adapter<D, I, C, V, Di, P, R>::difference_type
			iterator_adapter<D, I, C, V, Di, P, R>::Distance(const iterator_adapter &other) const
		{
			return iter - other.iter;
		}

		// comparison
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_adapter<D, I, C, V, Di, P, R>::Equal(const iterator_adapter &other) const
		{
			return iter == other.iter;
		}
		template <typename D, typename I, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_adapter<D, I, C, V, Di, P, R>::Less(const iterator_adapter &other) const
		{
			return iter < other.iter;
		}
	}
}
