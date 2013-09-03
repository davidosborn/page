#ifndef    page_local_log_filter_PrefixFilter_hpp
#   define page_local_log_filter_PrefixFilter_hpp

#	include "LineFilter.hpp"

namespace page
{
	namespace log
	{
		class PrefixFilter : public LineFilter
		{
			/*-------------+
			| constructors |
			+-------------*/

			using LineFilter::LineFilter;

			/*------------------+
			| virtual functions |
			+------------------*/

			private:
			virtual std::string GetPrefix() const = 0;

			/*--------------------------+
			| LineFilter implementation |
			+--------------------------*/

			private:
			std::string FilterLine(const std::string &) const override final;
		};
	}
}

#endif
