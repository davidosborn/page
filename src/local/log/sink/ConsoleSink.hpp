#ifndef    page_local_log_sink_ConsoleSink_hpp
#   define page_local_log_sink_ConsoleSink_hpp

#	include <memory> // shared_ptr

#	include "Sink.hpp"

namespace page
{
	namespace gui
	{
		class Console;
	}

	namespace log
	{
		class ConsoleSink final : public Sink
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			explicit ConsoleSink(const std::string &title);

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoWrite(const std::string &) override final;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::shared_ptr<gui::Console> console;
		};
	}
}

#endif
