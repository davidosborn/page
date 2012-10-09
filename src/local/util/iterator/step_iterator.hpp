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

// stepping iterator adapter

#ifndef    page_local_util_iterator_step_iterator_hpp
#   define page_local_util_iterator_step_iterator_hpp

#	include <iterator> // iterator_traits
#	include "iterator_adapter.hpp"

namespace page
{
	namespace util
	{
		template <typename Iterator, int step = 1> struct step_iterator :
			iterator_adapter<step_iterator<Iterator, step>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference>
		{
			template <typename, int> friend class step_iterator;

			friend class iterator_facade<step_iterator<Iterator, step>,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference>;

			typedef iterator_adapter<step_iterator<Iterator, step>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::reference reference;

			// constructors
			step_iterator();
			explicit step_iterator(const Iterator &);
			template <typename Iterator2>
				step_iterator(const step_iterator<Iterator2, step> &);

			private:
			// arithmetic
			void Increment() override;
			void Decrement() override;
			void Advance(difference_type) override;

			// distance
			difference_type Distance(const step_iterator &) const override;
		};

		// reverse arithmetic
		template <typename Iterator, int step>
			step_iterator<Iterator, step> operator +(
				typename step_iterator<Iterator, step>::difference_type,
				const step_iterator<Iterator, step> &);

		// factory function template
		template <int step, typename Iterator>
			inline step_iterator<Iterator, step> make_step_iterator(Iterator iter)
			{ return step_iterator<Iterator, step>(iter); }
	}
}

#	include "step_iterator.tpp"
#endif
