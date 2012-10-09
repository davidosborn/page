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

#ifndef    page_local_util_iterator_iterator_adapter_hpp
#   define page_local_util_iterator_iterator_adapter_hpp

#	include "iterator_facade.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A base class for iterator adapters.
		 *
		 * To create a new iterator-adapter class, inherit from this class, make
		 * this class a friend, and override the protected member functions to
		 * alter the behaviour.
		 */
		template <
			typename Derived,
			typename Iterator,
			typename Category,
			typename Value,
			typename DistanceT = std::ptrdiff_t,
			typename Pointer   = Value *,
			typename Reference = Value &>
			struct iterator_adapter :
				iterator_facade<Derived, Category, Value, DistanceT, Pointer, Reference>
		{
			friend class iterator_facade<Derived, Category, Value, DistanceT, Pointer, Reference>;

			typedef iterator_facade<Derived, Category, Value, DistanceT, Pointer, Reference> Base;

			typedef Iterator iterator_type;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::reference       reference;

			// constructors
			iterator_adapter();
			explicit iterator_adapter(const iterator_type &);

			// base access
			iterator_type     base() const;
			operator iterator_type() const;

			protected:
			// dereference
			reference Dereference() const;

			// arithmetic
			void Increment();
			void Decrement();
			void Advance(difference_type);

			// distance
			difference_type Distance(const iterator_adapter &) const;

			// comparison
			bool Equal(const iterator_adapter &) const;
			bool Less (const iterator_adapter &) const;

			iterator_type iter;
		};
	}
}

#	include "iterator_adapter.tpp"
#endif
