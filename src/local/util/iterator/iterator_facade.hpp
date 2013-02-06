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

#ifndef    page_local_util_iterator_iterator_facade_hpp
#   define page_local_util_iterator_iterator_facade_hpp

#	include <cstddef> // ptrdiff_t
#	include <iterator> // iterator
#	include <type_traits> // remove_cv

namespace page
{
	namespace util
	{
		/**
		 * A standard-library-esque iterator-facade designed to work with
		 * @c iterator_adapter.
		 *
		 * This class should usually not be inherited from directly.  When
		 * possible, use @c iterator_adapter instead.
		 */
		template <
			typename Derived,
			typename Category,
			typename Value,
			typename Distance  = std::ptrdiff_t,
			typename Pointer   = Value *,
			typename Reference = Value &>
			struct iterator_facade :
				std::iterator<Category,
					typename std::remove_cv<Value>::type,
					Distance, Pointer, Reference>
		{
			typedef std::iterator<Category, Value, Distance, Pointer, Reference> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::pointer pointer;
			typedef typename Base::reference reference;

			// dereference
			reference operator *() const;
			pointer operator ->() const;
			reference operator [](difference_type) const;

			// increment/decrement
			Derived &operator ++();
			Derived &operator --();
			Derived operator ++(int);
			Derived operator --(int);

			// arithmetic
			Derived operator +(difference_type) const;
			Derived operator -(difference_type) const;

			// distance
			difference_type operator -(const Derived &) const;

			// comparison
			bool operator  <(const Derived &) const;
			bool operator  >(const Derived &) const;
			bool operator <=(const Derived &) const;
			bool operator >=(const Derived &) const;
			bool operator ==(const Derived &) const;
			bool operator !=(const Derived &) const;

			// arithmetic assignment
			Derived &operator +=(difference_type);
			Derived &operator -=(difference_type);
		};

		// reverse arithmetic
		template <typename D, typename C, typename V, typename Di, typename P, typename R>
			D operator +(
				typename iterator_facade<D, C, V, Di, P, R>::difference_type,
				const iterator_facade<D, C, V, Di, P, R> &);
	}
}

#	include "iterator_facade.tpp"
#endif
