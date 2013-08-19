#ifndef    page_system_boost_mpl_flatten_hpp
#   define page_system_boost_mpl_flatten_hpp

#	include <boost/mpl/begin_end.hpp> // end
#	include <boost/mpl/clear.hpp>
#	include <boost/mpl/inserter.hpp>
#	include <boost/mpl/insert_range.hpp>
#	include <boost/mpl/is_sequence.hpp>
#	include <boost/mpl/quote.hpp>
#	include <boost/mpl/single_view.hpp>
#	include <boost/mpl/transform.hpp>

namespace boost { namespace mpl
{
	/**
	 * Recursively expands sub-sequences to produce a flat sequence.
	 */
	template <
		typename Sequence,
		bool = is_sequence<Sequence>::value>
			struct flatten
	{
		typedef
			typename transform<
				Sequence,
				quote1<flatten>,
				inserter<
					typename clear<Sequence>::type,
					insert_range<_1, end<_1>, _2>>
				>::type type;
	};

	template <typename T>
		struct flatten<T, false>
	{
		typedef single_view<T> type;
	};
}}

#endif
