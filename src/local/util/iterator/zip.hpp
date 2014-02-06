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
