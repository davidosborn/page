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

#ifndef    page_util_iterator_range_hpp
#   define page_util_iterator_range_hpp

#	include <utility> // declval, pair
#	include "../type_traits.hpp" // ENABLE_IF, is_range

namespace page
{
	namespace util
	{
		/**
		 * A iterator-range wrapper to provide more comprehensive support for
		 * range-based for-loops.
		 */
		template <typename Iterator> struct range
		{
			template <typename> friend class range;

			// ISO/IEC N3242 23.2.1.4 Container requirements
			typedef typename std::iterator_traits<Iterator>::value_type      value_type;
			typedef typename std::iterator_traits<Iterator>::reference       reference;
			typedef typename std::add_const<reference>::type                 const_reference;
			typedef Iterator                                                 iterator;
			typedef Iterator                                                 const_iterator;
			typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
			typedef typename std::make_unsigned<difference_type>::type       size_type;

			// constructors
			range();
			range(Iterator first, Iterator last);
			range(const std::pair<Iterator, Iterator> &);
			template <typename T> explicit range(T &, ENABLE_IF(is_range<T>::value));
			template <typename T> explicit range(const T &, ENABLE_IF(is_range<T>::value));
			template <typename Iterator2> range(const range<Iterator2> &);

			// iterators
			Iterator begin() const noexcept;
			Iterator end()   const noexcept;

			// observers
			bool empty() const noexcept;

			// conversion
			operator const std::pair<Iterator, Iterator> &() const;

			private:
			std::pair<Iterator, Iterator> pair;
		};

		// observers
		template <typename Iterator>
			bool in(const range<Iterator> &, Iterator);

		// factory functions
		template <typename Iterator>
			range<Iterator> make_range(Iterator first, Iterator last);
		template <typename Iterator>
			range<Iterator> make_range(const std::pair<Iterator, Iterator> &);
		template <typename T>
			auto make_range(T &) -> range<decltype(std::begin(std::declval<T &>()))>;
		template <typename T>
			auto make_range(const T &) -> range<decltype(std::begin(std::declval<const T &>()))>;
	}
}

#	include "range.tpp"
#endif
