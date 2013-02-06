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
		// dereference
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_facade<D, C, V, Di, P, R>::reference
			iterator_facade<D, C, V, Di, P, R>::operator *() const
		{
			return static_cast<const D &>(*this).Dereference();
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_facade<D, C, V, Di, P, R>::pointer
			iterator_facade<D, C, V, Di, P, R>::operator ->() const
		{
			return &static_cast<const D &>(*this).Dereference();
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_facade<D, C, V, Di, P, R>::reference
			iterator_facade<D, C, V, Di, P, R>::operator [](difference_type i) const
		{
			return static_cast<const D &>(*this + i).Dereference();
		}

		// increment/decrement
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D &iterator_facade<D, C, V, Di, P, R>::operator ++()
		{
			static_cast<D &>(*this).Increment();
			return static_cast<D &>(*this);
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D &iterator_facade<D, C, V, Di, P, R>::operator --()
		{
			static_cast<D &>(*this).Decrement();
			return static_cast<D &>(*this);
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D iterator_facade<D, C, V, Di, P, R>::operator ++(int)
		{
			D pre(static_cast<D &>(*this));
			static_cast<D &>(*this).Increment();
			return pre;
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D iterator_facade<D, C, V, Di, P, R>::operator --(int)
		{
			D pre(static_cast<D &>(*this));
			static_cast<D &>(*this).Decrement();
			return pre;
		}

		// arithmetic
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D iterator_facade<D, C, V, Di, P, R>::operator +(difference_type n) const
		{
			D r(static_cast<const D &>(*this));
			r.Advance(n);
			return r;
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D iterator_facade<D, C, V, Di, P, R>::operator -(difference_type n) const
		{
			D r(static_cast<const D &>(*this));
			r.Advance(-n);
			return r;
		}

		// distance
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			typename iterator_facade<D, C, V, Di, P, R>::difference_type
			iterator_facade<D, C, V, Di, P, R>::operator -(const D &other) const
		{
			return static_cast<const D &>(*this).Distance(other);
		}

		// comparison
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_facade<D, C, V, Di, P, R>::operator ==(const D &other) const
		{
			return static_cast<const D &>(*this).Equal(other);
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_facade<D, C, V, Di, P, R>::operator !=(const D &other) const
		{
			return !static_cast<const D &>(*this).Equal(other);
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_facade<D, C, V, Di, P, R>::operator <(const D &other) const
		{
			return static_cast<const D &>(*this).Less(other);
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_facade<D, C, V, Di, P, R>::operator >(const D &other) const
		{
			return other.Less(static_cast<const D &>(*this));
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_facade<D, C, V, Di, P, R>::operator <=(const D &other) const
		{
			return !other.Less(static_cast<const D &>(*this));
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			bool iterator_facade<D, C, V, Di, P, R>::operator >=(const D &other) const
		{
			return !static_cast<const D &>(*this).Less(other);
		}

		// arithmetic assignment
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D &iterator_facade<D, C, V, Di, P, R>::operator +=(difference_type n)
		{
			static_cast<D &>(*this).Advance(n);
			return static_cast<D &>(*this);
		}
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D &iterator_facade<D, C, V, Di, P, R>::operator -=(difference_type n)
		{
			static_cast<D &>(*this).Advance(-n);
			return static_cast<D &>(*this);
		}

		// reverse arithmetic
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D operator +(typename iterator_facade<D, C, V, Di, P, R>::difference_type n, const iterator_facade<D, C, V, Di, P, R> &iter)
		{
			iter + n;
		}
	}
}
