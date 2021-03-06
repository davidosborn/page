/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_util_iterator_indirect_iterator_hpp
#   define page_util_iterator_indirect_iterator_hpp

#	include <iterator> // begin, end, iterator{,_traits}

#	include "../type_traits/pointer.hpp" // remove_indirection
#	include "../type_traits/range.hpp" // range_traits
#	include "range.hpp"

namespace page { namespace util
{
	/**
	 * An iterator that dereferences the underlying iterator twice.  It is
	 * equivalent to boost::indirect_iterator.
	 *
	 * @note This class has been provided as a work around for
	 *       <a href="https://svn.boost.org/trac/boost/ticket/5965">Boost bug
	 *       #5965</a>.
	 */
	template <typename Iterator>
		class indirect_iterator :
			public std::iterator<
				typename std::iterator_traits<Iterator>::iterator_category,
				typename remove_indirection<
					typename remove_indirection<Iterator>::type
					>::type
				>
	{
		template <typename>
			friend class indirect_iterator;

		public:
		using typename indirect_iterator::iterator::difference_type;
		using typename indirect_iterator::iterator::pointer;
		using typename indirect_iterator::iterator::reference;

		/*-------------+
		| constructors |
		+-------------*/

		indirect_iterator(const Iterator &iter) :
			iter(iter) {}

		template <typename OtherIterator>
			indirect_iterator(const indirect_iterator<OtherIterator> &other) :
				indirect_iterator(other.iter) {}

		/*-------------------+
		| iterator semantics |
		+-------------------*/

		reference operator *() const
		{
			return **iter;
		}

		pointer operator ->() const
		{
			return *iter;
		}

		indirect_iterator &operator ++()
		{
			++iter;
			return *this;
		}

		indirect_iterator &operator --()
		{
			--iter;
			return *this;
		}

		indirect_iterator &operator ++(int)
		{
			indirect_iterator r(*this);
			++*this;
			return r;
		}

		indirect_iterator &operator --(int)
		{
			indirect_iterator r(*this);
			--*this;
			return r;
		}

		bool operator +=(difference_type n)
		{
			std::advance(iter, n);
			return *this;
		}

		bool operator -=(difference_type n)
		{
			return *this += -n;
		}

		indirect_iterator operator +(difference_type n) const
		{
			return indirect_iterator(std::advance(iter, n));
		}

		indirect_iterator operator -(difference_type n) const
		{
			return *this + -n;
		}

		difference_type operator -(const indirect_iterator &other) const
		{
			return iter - other.iter;
		}

		bool operator ==(const indirect_iterator &other) const
		{
			return iter == other.iter;
		}

		bool operator !=(const indirect_iterator &other) const
		{
			return iter != other.iter;
		}

		bool operator <=(const indirect_iterator &other) const
		{
			return iter <= other.iter;
		}

		bool operator >=(const indirect_iterator &other) const
		{
			return iter >= other.iter;
		}

		bool operator <(const indirect_iterator &other) const
		{
			return iter < other.iter;
		}

		bool operator >(const indirect_iterator &other) const
		{
			return iter > other.iter;
		}

		/*-------------+
		| data members |
		+-------------*/

		private:
		Iterator iter;
	};

	/*------------------+
	| factory functions |
	+------------------*/

	/**
	 * Factory function (for convenience).
	 */
	template <typename Iterator>
		indirect_iterator<Iterator> make_indirect_iterator(const Iterator &iter)
			{ return indirect_iterator<Iterator>(iter); }

	/**
	 * Returns a range over the iterators of the original range wrapped in @c
	 * indirect_iterator.  This is equivalent to @c boost::adaptors::indirect.
	 *
	 * @note This function has been provided as a work around for
	 *       <a href="https://svn.boost.org/trac/boost/ticket/5965">Boost bug
	 *       #5965</a>.
	 */
	template <typename Range,
		typename Iterator = typename range_traits<Range>::iterator>
		range<indirect_iterator<Iterator>> make_indirect_range(const Range &range)
	{
		return make_range(
			make_indirect_iterator(std::begin(range)),
			make_indirect_iterator(std::end  (range)));
	}
}}

#endif
