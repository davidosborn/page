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

#ifndef    page_local_util_iterator_ring_iterator_hpp
#   define page_local_util_iterator_ring_iterator_hpp

#	include <iterator> // iterator_traits

#	include "iterator_adapter.hpp"
#	include "range.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A circular iterator adapter.
		 */
		template <typename Iterator> struct ring_iterator :
			iterator_adapter<ring_iterator<Iterator>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference>
		{
			template <typename, int> friend class ring_iterator;

			friend class iterator_facade<ring_iterator<Iterator>,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference>;

			typedef iterator_adapter<ring_iterator<Iterator>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::reference reference;

			// constructors
			ring_iterator(Iterator first, Iterator last);
			ring_iterator(Iterator first, Iterator last, Iterator current);
			ring_iterator(const range<Iterator> &);
			ring_iterator(const range<Iterator> &, Iterator current);
			template <typename Iterator2>
				ring_iterator(const ring_iterator<Iterator2> &);

			private:
			// arithmetic
			void Increment();
			void Decrement();
			void Advance(difference_type);

			util::range<Iterator> range;
		};

		// factory functions
		template <typename Iterator>
			inline ring_iterator<Iterator> make_ring_iterator(Iterator first, Iterator last);
		template <typename Iterator>
			inline ring_iterator<Iterator> make_ring_iterator(Iterator first, Iterator last, Iterator current);
		template <typename Iterator>
			inline ring_iterator<Iterator> make_ring_iterator(const range<Iterator> &);
		template <typename Iterator>
			inline ring_iterator<Iterator> make_ring_iterator(const range<Iterator> &, Iterator current);
	}
}

#	include "ring_iterator.tpp"
#endif
