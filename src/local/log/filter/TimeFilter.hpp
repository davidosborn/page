#ifndef    page_local_log_filter_TimeFilter_hpp
#   define page_local_log_filter_TimeFilter_hpp

#	include "PrefixFilter.hpp"

namespace page
{
	namespace log
	{
		/**
		 * Prefixes each line with the current time.
		 */
		class TimeFilter final : public PrefixFilter
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			using TimeFilter::TimeFilter;

			/*----------------------------+
			| PrefixFilter implementation |
			+----------------------------*/

			private:
			std::string GetPrefix() const override final;
		};
	}
}

#endif
