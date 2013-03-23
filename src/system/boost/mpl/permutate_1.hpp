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

#ifndef    page_system_boost_mpl_permutate_1_hpp
#   define page_system_boost_mpl_permutate_1_hpp

#	include <boost/mpl/advance.hpp>
#	include <boost/mpl/begin_end.hpp>
#	include <boost/mpl/copy.hpp> // reverse_copy
#	include <boost/mpl/erase.hpp>
#	include <boost/mpl/fold.hpp>
#	include <boost/mpl/front_inserter.hpp>
#	include <boost/mpl/insert.hpp>
#	include <boost/mpl/next.hpp>
#	include <boost/mpl/pair.hpp>
#	include <boost/mpl/vector.hpp>

namespace boost { namespace mpl
{
	/**
	 * Generates a sequence of first-level permutations for an ordered
	 * sequence of types.  A first-level permutation is the same as the
	 * original sequence with one element removed.
	 *
	 * The algorithm can be described in the following Python program:
	 * @code
	 * def permutations(L):
	 *     return [L[0:i] + L[i+1:] for i in range(len(L))]
	 * @endcode
	 */
	template <typename T>
		struct permutate_1
	{
		typedef
			typename reverse_copy<
				T,
				front_inserter<vector<>>
				>::type ExtensibleSequence;

		typedef
			typename first<
				typename fold<
					ExtensibleSequence,
					pair<
						vector<>,
						int_<0>>,
					pair<
						insert<
							first<_1>,
							end<first<_1>>,
							erase<
								ExtensibleSequence,
								advance<
									typename begin<ExtensibleSequence>::type,
									second<_1>>
								>
							>,
						next<second<_1>>>
					>::type
				>::type type;
	};
}}

#endif
