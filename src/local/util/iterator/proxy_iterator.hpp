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
