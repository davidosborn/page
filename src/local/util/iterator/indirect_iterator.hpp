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

// indirect iterator adapter
// dereferences the iterator twice
// inspired by Boost's indirect_iterator

#ifndef    page_util_iterator_indirect_iterator_hpp
#   define page_util_iterator_indirect_iterator_hpp

#	include <iterator> // iterator_traits
#	include <type_traits> // remove_cv
#	include "../type_traits.hpp" // remove_indirection
#	include "iterator_adapter.hpp"

namespace page
{
	namespace util
	{
		template <typename Iterator, typename Value =
			typename remove_indirection<typename std::iterator_traits<Iterator>::value_type>::type>
			struct indirect_iterator :
				iterator_adapter<indirect_iterator<Iterator, Value>, Iterator,
					typename std::iterator_traits<Iterator>::iterator_category,
					typename std::remove_cv<Value>::type,
					typename std::iterator_traits<Iterator>::difference_type,
					Value *, Value &>
		{
			template <typename, typename> friend class indirect_iterator;

			friend class iterator_facade<indirect_iterator<Iterator, Value>,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::remove_cv<Value>::type,
				typename std::iterator_traits<Iterator>::difference_type,
				Value *, Value &>;

			typedef iterator_adapter<indirect_iterator<Iterator, Value>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::remove_cv<Value>::type,
				typename std::iterator_traits<Iterator>::difference_type,
				Value *, Value &> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::reference reference;

			// constructors
			indirect_iterator();
			explicit indirect_iterator(const Iterator &);
			template <typename Iterator2, typename Value2>
				indirect_iterator(const indirect_iterator<Iterator2, Value2> &);

			private:
			// dereference
			reference Dereference() const override;
		};

		// reverse arithmetic
		template <typename Iterator, typename Value>
			indirect_iterator<Iterator, Value> operator +(
				typename indirect_iterator<Iterator, Value>::difference_type,
				const indirect_iterator<Iterator, Value> &);

		// factory function template
		template <typename Iterator>
			inline indirect_iterator<Iterator> make_indirect_iterator(Iterator iter)
			{ return indirect_iterator<Iterator>(iter); }
	}
}

#	include "indirect_iterator.tpp"
#endif
