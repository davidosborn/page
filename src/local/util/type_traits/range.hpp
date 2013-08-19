#ifndef    page_local_util_type_traits_range_hpp
#   define page_local_util_type_traits_range_hpp

#	include <iterator> // begin, end
#	include <type_traits>
#	include <utility> // declval

#	include "sfinae.hpp"

namespace page { namespace util
{
	/**
	 * Detects whether a type is a range, which includes any type for which @c
	 * std::begin(x) and @c std::end(x) are valid function calls.
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_range,
		decltype(
			std::begin(std::declval<T>()),
			std::end  (std::declval<T>()),
			std::declval<void>()))

	/**
	 * A collection of type traits for a range, comparable to @c
	 * std::iterator_traits.
	 */
	template <typename Range,
		typename Iterator = typename std::decay<decltype(std::begin(std::declval<Range>()))>::type>
		struct range_traits : std::iterator_traits<Iterator>
	{
		using iterator        = Iterator;
		using iterator_traits = std::iterator_traits<iterator>;
	};

	/**
	 * Detects whether @c const is propogated to the elements of a range.
	 *
	 * @note This type trait will only produce the correct result when the
	 *       elements are not themselves @c const.
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_const_propogated,
		typename std::enable_if<
			std::is_const<
				typename range_traits<const T>::value_type
				>::value
			>::type)
}}

#endif
