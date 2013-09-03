#ifndef    page_local_log_sink_FileSink_hpp
#   define page_local_log_sink_FileSink_hpp

#	include <fstream> // ofstream
#	include <string>

#	include "Sink.hpp"

namespace page
{
	namespace log
	{
		class FileSink final : public Sink
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			explicit FileSink(std::string path);

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoWrite(const std::string &) override final;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::ofstream fs;
		};
	}
}

#endif
