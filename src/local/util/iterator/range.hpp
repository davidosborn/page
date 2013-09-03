#ifndef    page_util_iterator_range_hpp
#   define page_util_iterator_range_hpp

#	include <iterator> // iterator_traits
#	include <type_traits> // add_const, make_unsigned
#	include <utility> // pair

#	include "../type_traits/range.hpp" // is_range, range_traits
#	include "../type_traits/sfinae.hpp" // ENABLE_IF

namespace page { namespace util
{
	/**
	 * A iterator-range wrapper to provide more comprehensive support for range-
	 * based for-loops and range-based programming in general.
	 *
	 * @note This class uses the <em>proxy pattern</em>.  The constness of the
	 *       proxy affects only the operations that modify the state of the
	 *       proxy, not those that modify the state of the targeted object.
	 *
	 * @note This class is mostly equivalent to @c boost::iterator_range, and
	 *       has been retained to get around problems using @c
	 *       boost::iterator_range with incomplete types.
	 */
	template <typename Iterator>
		class range
	{
		template <typename>
			friend class range;

		/*-----------------+
		| container traits |
		+-----------------*/

		public:
		using value_type             = typename std::iterator_traits<Iterator>::value_type;
		using reference              = typename std::iterator_traits<Iterator>::reference;
		using const_reference        = typename std::add_const<reference>::type;
		using iterator               = Iterator;
		using const_iterator         = Iterator;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using difference_type        = typename std::iterator_traits<Iterator>::difference_type;
		using size_type              = typename std::make_unsigned<difference_type>::type;

		/*-------------+
		| constructors |
		+-------------*/

		range();
		range(const iterator &first, const iterator &last);
		range(const std::pair<iterator, iterator> &);

		template <typename Range>
			explicit range(const Range &, ENABLE_IF(is_range<Range>::value));

		template <typename Iterator2>
			range(const range<Iterator2> &);

		/*----------+
		| iterators |
		+----------*/

		iterator const&  begin()  const noexcept;
		iterator const&  end()    const noexcept;
		reverse_iterator rbegin() const;
		reverse_iterator rend()   const;

		/*----------+
		| observers |
		+----------*/

		/**
		 * @return @a iter.
		 */
		operator const std::pair<iterator, iterator> &() const noexcept;

		/**
		 * @return @c true if the range is empty.
		 */
		bool empty() const noexcept;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * A @c std::pair containing the first and last iterators.
		 */
		const std::pair<iterator, iterator> iter;
	};

	/*------------------+
	| factory functions |
	+------------------*/

	template <typename Iterator>
		range<Iterator> make_range(Iterator first, Iterator last);

	template <typename Iterator>
		range<Iterator> make_range(const std::pair<Iterator, Iterator> &);

	template <typename Range,
		typename Iterator = typename range_traits<Range>::iterator>
		range<Iterator> make_range(const Range &);
}}

#	include "range.tpp"
#endif
