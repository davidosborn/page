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
