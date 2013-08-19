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
