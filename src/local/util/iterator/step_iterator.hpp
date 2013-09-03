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
