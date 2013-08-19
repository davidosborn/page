#ifndef    page_util_iterator_proxy_iterator_hpp
#   define page_util_iterator_proxy_iterator_hpp

#	include <utility> // declval

#	include <boost/iterator/transform_iterator.hpp>

namespace page { namespace util
{
	/**
	 * Equivalent to @c boost::transform_iterator, except that dereferencing
	 * yields a copy, not a reference.
	 */
	template <typename Iterator, typename UnaryOperation>
		using proxy_iterator = boost::transform_iterator<UnaryOperation, Iterator,
			decltype(std::declval<UnaryOperation>()(*std::declval<Iterator>()))>;

	/**
	 * Factory function (for convenience).
	 */
	template <typename Iterator, typename UnaryOperation>
		proxy_iterator<Iterator, UnaryOperation>
		make_proxy_iterator(const Iterator &iter, const UnaryOperation &op)
			{ return proxy_iterator<Iterator, UnaryOperation>(iter, op); }
}}

#endif
