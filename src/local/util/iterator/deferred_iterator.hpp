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

#ifndef    page_local_util_iterator_deferred_iterator_hpp
#   define page_local_util_iterator_deferred_iterator_hpp

#	include <iterator> // iterator_traits

#	include "iterator_adapter.hpp"

namespace page
{
	namespace util
	{
		template <typename Iterator> struct deferred_iterator :
			iterator_adapter<deferred_iterator<Iterator>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category, Iterator,
				typename std::iterator_traits<Iterator>::difference_type>
		{
			friend class iterator_facade<deferred_iterator<Iterator>,
				typename std::iterator_traits<Iterator>::iterator_category, Iterator,
				typename std::iterator_traits<Iterator>::difference_type>;

			typedef iterator_adapter<deferred_iterator<Iterator>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category, Iterator,
				typename std::iterator_traits<Iterator>::difference_type> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::reference reference;

			// constructors
			deferred_iterator();
			explicit deferred_iterator(const Iterator &);
			template <typename Iterator2>
				deferred_iterator(const deferred_iterator<Iterator2> &);

			private:
			// dereference
			reference Dereference() const override;
		};

		// reverse arithmetic
		template <typename Iterator>
			deferred_iterator<Iterator> operator +(
				typename deferred_iterator<Iterator>::difference_type,
				const deferred_iterator<Iterator> &);

		// factory function template
		template <typename Iterator>
			inline deferred_iterator<Iterator> make_deferred_iterator(Iterator iter)
			{ return deferred_iterator<Iterator>(iter); }
	}
}

#	include "deferred_iterator.tpp"
#endif
