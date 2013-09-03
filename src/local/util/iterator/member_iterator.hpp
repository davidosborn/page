#ifndef    page_util_iterator_member_iterator_hpp
#   define page_util_iterator_member_iterator_hpp

#	include <iterator> // begin, end, iterator_traits

#	include <boost/iterator/iterator_adaptor.hpp>

#	include "../type_traits/const_volatile.hpp" // copy_cv
#	include "../type_traits/pointer.hpp" // remove_indirection
#	include "../type_traits/range.hpp" // range_traits
#	include "range.hpp"

namespace page { namespace util
{
	/**
	 * An iterator that returns the value of a member, where the underlying
	 * iterator points to an object that contains the member.
	 *
	 * @sa Based on an idea by Andrei Tovtchigretchko,
	 *     http://lists.boost.org/Archives/boost/2000/01/1843.php.
	 */
	template <typename Iterator, typename Value>
		class member_iterator :
			public boost::iterator_adaptor<
				member_iterator<Iterator, Value>, Iterator,
				typename copy_cv<Value, typename remove_indirection<Iterator>::type>::type>
	{
		template <typename, typename>
			friend class member_iterator;

		friend class boost::iterator_core_access;

		public:
		/**
		 * The class that contains the member.
		 */
		typedef typename std::iterator_traits<Iterator>::value_type member_of;

		/*-------------+
		| constructors |
		+-------------*/

		member_iterator(const Iterator &iter, Value member_of::*member) :
			member_iterator::iterator_adaptor(iter), member(member) {}

		template <typename OtherIterator, typename OtherValue>
			member_iterator(const member_iterator<OtherIterator, OtherValue> &other) :
				member_iterator(other.base(), other.member) {}

		/*--------------------------------+
		| iterator_adapter implementation |
		+--------------------------------*/

		private:
		typename member_iterator::reference dereference() const
		{
			return (*this->base()).*member;
		}

		/*-------------+
		| data members |
		+-------------*/

		Value member_of::*member;
	};

	/*------------------+
	| factory functions |
	+------------------*/

	/**
	 * Factory function (for convenience).
	 */
	template <typename Iterator, typename Value>
		member_iterator<Iterator, Value>
		make_member_iterator(const Iterator &iter,
			Value member_iterator<Iterator, Value>::member_of::*member)
				{ return member_iterator<Iterator, Value>(iter, member); }

	/**
	 * @return A range over the iterators of the original range wrapped in @c
	 * member_iterator.
	 */
	template <typename Range, typename Value,
		typename Iterator = typename range_traits<Range>::iterator>
		range<member_iterator<Iterator, Value>> make_member_range(const Range &range,
			Value member_iterator<Iterator, Value>::member_of::*member)
	{
		return make_range(
			make_member_iterator(std::begin(range), member),
			make_member_iterator(std::end  (range), member));
	}
}}

#endif
