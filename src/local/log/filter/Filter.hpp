#ifndef    page_local_log_filter_Filter_hpp
#   define page_local_log_filter_Filter_hpp

#	include "../stream/BranchableStream.hpp"

namespace page
{
	namespace log
	{
		/**
		 * The base class for filters.
		 */
		class Filter : public BranchableStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			using BranchableStream::BranchableStream;

			/*------------------+
			| virtual functions |
			+------------------*/

			private:
			virtual std::string DoFilter(const std::string &) const = 0;

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoWrite(const std::string &) override final;
		};
	}
}

#endif
