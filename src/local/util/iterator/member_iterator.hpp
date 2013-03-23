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

#ifndef    page_local_util_iterator_member_iterator_hpp
#   define page_local_util_iterator_member_iterator_hpp

#	include <iterator> // iterator_traits
#	include <type_traits> // remove_cv

#	include "../type_traits/const_volatile.hpp" // copy_cv
#	include "../type_traits/pointer.hpp" // remove_indirection
#	include "iterator_adapter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A member iterator adapter, for providing access to a member of the
		 * @c value_type of an iterator.
		 *
		 * @sa Based on idea by Andrei Tovtchigretchko,
		 *     http://lists.boost.org/Archives/boost/2000/01/1843.php.
		 */
		template <typename Iterator, typename Value>
			struct member_iterator :
				iterator_adapter<member_iterator<Iterator, Value>, Iterator,
					typename std::iterator_traits<Iterator>::iterator_category,
					typename std::remove_cv<Value>::type,
					typename std::iterator_traits<Iterator>::difference_type,
					typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type *,
					typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type &>
		{
			template <typename, typename> friend class member_iterator;

			friend class iterator_facade<member_iterator<Iterator, Value>,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::remove_cv<Value>::type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type *,
				typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type &>;

			typedef iterator_adapter<member_iterator<Iterator, Value>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::remove_cv<Value>::type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type *,
				typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type &> Base;

			typedef typename std::iterator_traits<Iterator>::value_type member_of;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::reference reference;

			// constructors
			member_iterator(const Iterator &, Value member_of::*);
			template <typename Iterator2, typename Value2>
				member_iterator(const member_iterator<Iterator2, Value2> &);

			private:
			// dereference
			reference Dereference() const;

			Value member_of::*member;
		};

		// factory function template
		template <typename Iterator, typename Value>
			inline member_iterator<Iterator, Value> make_member_iterator(Iterator, Value member_iterator<Iterator, Value>::member_of::*);
	}
}

#	include "member_iterator.tpp"
#endif
