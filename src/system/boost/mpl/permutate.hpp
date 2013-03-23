/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_system_boost_mpl_permutate_hpp
#   define page_system_boost_mpl_permutate_hpp

#	include <boost/mpl/back_inserter.hpp>
#	include <boost/mpl/copy.hpp>
#	include <boost/mpl/empty.hpp>
#	include <boost/mpl/front.hpp>
#	include <boost/mpl/join.hpp>
#	include <boost/mpl/pop_front.hpp>
#	include <boost/mpl/transform.hpp>
#	include <boost/mpl/vector.hpp>

namespace boost { namespace mpl
{
	/**
	 * Generates a sequence of permutations for an ordered sequence
	 * of types.
	 *
	 * The algorithm can be described in the following Python program:
	 * @code
	 * def permutations(L):
	 *     r = []
	 *     if L:
	 *         root = L[0:1]
	 *         stem = L[1:]
	 *         perm = permutations(stem)
	 *         r.extend([root + p for p in perm])
	 *         r.append(root)
	 *         r.extend(perm)
	 *     return r
	 * @endcode
	 */
	template <typename T, bool = empty<T>::value>
		struct permutate
	{
		private:
		typedef vector<typename front<T>::type> Root;
		typedef typename pop_front<T>::type Stem;
		typedef typename permutate<Stem>::type Permutations;
		typedef typename transform<
			Permutations,
			copy<_1, back_inserter<Root>>
			>::type RootPermutations;

		public:
		typedef
			typename join<
				RootPermutations,
				vector<Root>,
				Permutations>::type type;
	};

	template <typename T>
		struct permutate<T, true>
	{
		typedef T type;
	};
}}

#endif
