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

#ifndef    page_util_iterator_zip_hpp
#   define page_util_iterator_zip_hpp

#	include <iterator> // begin

#	include <boost/iterator/zip_iterator.hpp>
#	include <boost/range/iterator_range.hpp>
#	include <boost/tuple/tuple.hpp>

namespace page { namespace util
{
	/**
	 * Returns a range for use in range-based for-loops, where dereferencing the
	 * iterator yields a tuple containing the results of dereferencing each of
	 * the zipped iterators.
	 */
	template <typename... Ranges>
		auto zip(Ranges &&... ranges) ->
			boost::iterator_range<
				boost::zip_iterator<
					boost::tuple<
						decltype(std::begin(std::forward<Ranges>(ranges)))...
						>
					>
				>
	{
		return
			boost::iterator_range<
				boost::zip_iterator<
					boost::tuple<
						decltype(std::begin(std::forward<Ranges>(ranges)))...
						>
					>
				>(
			boost::make_zip_iterator(boost::make_tuple(std::begin(std::forward<Ranges>(ranges))...)),
			boost::make_zip_iterator(boost::make_tuple(std::end  (std::forward<Ranges>(ranges))...)));
	}
}}

#endif
