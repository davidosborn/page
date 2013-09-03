#ifndef    page_local_log_filter_LineFilter_hpp
#   define page_local_log_filter_LineFilter_hpp

#	include "Filter.hpp"

namespace page
{
	namespace log
	{
		/**
		 * The base class for filters which process newline-terminated lines,
		 * rather than individual characters.
		 */
		class LineFilter : public Filter
		{
			/*-------------+
			| constructors |
			+-------------*/

			using Filter::Filter;

			/*------------------+
			| virtual functions |
			+------------------*/

			private:
			virtual std::string FilterLine(const std::string &) const = 0;

			/*----------------------+
			| Filter implementation |
			+----------------------*/

			private:
			std::string DoFilter(const std::string &) const override final;

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoClear() override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A buffer holding the last incomplete line.  A line is complete
			 * when it is terminated by a newline character.
			 */
			mutable std::string buffer;
		};
	}
}

#endif
