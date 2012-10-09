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

#ifndef    page_local_util_iterator_chain_iterator_hpp
#   define page_local_util_iterator_chain_iterator_hpp

#	include <initializer_list>
#	include <iterator> // iterator_traits
#	include <utility> // pair
#	include <vector>
#	include "iterator_adapter.hpp"
#	include "range.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A chaining iterator adapter, for concatenating multiple ranges.
		 *
		 * @note This class only supports a single iterator type.  A useful
		 *       extension would be to support multiple iterator types via
		 *       variadic template arguments.
		 */
		template <typename Iterator> struct chain_iterator :
			iterator_adapter<chain_iterator<Iterator>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference>
		{
			template <typename> friend class chain_iterator;

			friend class iterator_facade<chain_iterator<Iterator>,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference>;

			typedef iterator_adapter<chain_iterator<Iterator>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category,
				typename std::iterator_traits<Iterator>::value_type,
				typename std::iterator_traits<Iterator>::difference_type,
				typename std::iterator_traits<Iterator>::pointer,
				typename std::iterator_traits<Iterator>::reference> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::iterator_type iterator_type;
			typedef typename Base::difference_type difference_type;

			// constructors
			explicit chain_iterator(Iterator);
			chain_iterator(std::initializer_list<std::pair<Iterator, Iterator>>, Iterator);
			chain_iterator(std::initializer_list<range<Iterator>>, Iterator);
			template <typename Iterator2>
				chain_iterator(const chain_iterator<Iterator2> &);

			private:
			// initialization
			void Init();
			
			// arithmetic
			void Increment() override;
			void Decrement() override;
			void Advance(difference_type) override;

			// distance
			difference_type Distance(const chain_iterator &) const override;

			// comparison
			bool Less(const chain_iterator &) const override;
			
			// mapping
			typedef std::vector<range<Iterator>> Ranges;
			typename Ranges::const_iterator WhichRange(const Iterator &) const;

			using Base::iter;
			/**
			 * 
			 * @note If @c ranges is empty, the iterator may represent the end
			 *       of a chain of ranges, or it could represent the beginning
			 *       of a chain containing a single range.
			 */
			Ranges ranges;
			Iterator tail;
			typename Ranges::const_iterator currentRange;
		};

		// factory functions
		template <typename Iterator>
			inline chain_iterator<Iterator> make_chain_iterator(Iterator);
		template <typename Iterator>
			inline chain_iterator<Iterator> make_chain_iterator(std::initializer_list<std::pair<Iterator, Iterator>>, Iterator);
		template <typename Iterator>
			inline chain_iterator<Iterator> make_chain_iterator(std::initializer_list<range<Iterator>>, Iterator);
	}
}

#	include "chain_iterator.tpp"
#endif
