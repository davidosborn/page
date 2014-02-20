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

#ifndef    page_util_iterator_step_iterator_hpp
#   define page_util_iterator_step_iterator_hpp

#	include <iterator> // advance

#	include <boost/iterator/iterator_adaptor.hpp>

namespace page { namespace util
{
	/**
	 * An iterator adaptor that moves in steps.
	 */
	template <typename Iterator, int step = 1>
		class step_iterator :
			public boost::iterator_adaptor<step_iterator<Iterator, step>, Iterator>
	{
		friend class boost::iterator_core_access;

		// inherit constructors
		using step_iterator::iterator_adaptor::iterator_adaptor;

		/*--------------------------------+
		| iterator_adapter implementation |
		+--------------------------------*/

		void increment()
		{
			std::advance(this->base_reference(), step);
		}

		void decrement()
		{
			std::advance(this->base_reference(), -step);
		}

		void advance(typename step_iterator::difference_type n)
		{
			std::advance(this->base_reference(), n * step);
		}
	};

	/*------------------+
	| factory functions |
	+------------------*/

	/**
	 * Factory function (for convenience).
	 */
	template <int step, typename Iterator>
		step_iterator<Iterator> make_step_iterator(const Iterator &iter)
			{ return step_iterator<Iterator, step>(iter); }
}}

#endif
