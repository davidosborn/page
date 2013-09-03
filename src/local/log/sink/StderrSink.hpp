#ifndef    page_local_log_sink_StderrSink_hpp
#   define page_local_log_sink_StderrSink_hpp

#	include "Sink.hpp"

namespace page
{
	namespace log
	{
		class StderrSink final : public Sink
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			StderrSink();

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoWrite(const std::string &) override final;
			void DoFlush() override final;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::streambuf &streambuf;
		};
	}
}

#endif
