#ifndef    page_system_boost_mpl_join_hpp
#   define page_system_boost_mpl_join_hpp

#	include <boost/mpl/copy.hpp> // reverse_copy
#	include <boost/mpl/front_inserter.hpp>
#	include <boost/mpl/vector.hpp>

namespace boost { namespace mpl
{
	/**
	 * Recursively concatenates multiple sequences into a single
	 * sequence.
	 */
	template <typename... Sequences>
		struct join;

	template <typename Sequence, typename... RemainingSequences>
		struct join<Sequence, RemainingSequences...>
	{
		typedef
			typename reverse_copy<
				Sequence,
				front_inserter<
					typename join<RemainingSequences...>::type>
				>::type type;
	};

	template <>
		struct join<>
	{
		typedef vector<> type;
	};
}}

#endif
