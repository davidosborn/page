#ifndef    page_local_log_sink_StdoutSink_hpp
#   define page_local_log_sink_StdoutSink_hpp

#	include "Sink.hpp"

namespace page
{
	namespace log
	{
		class StdoutSink final : public Sink
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			StdoutSink();

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
