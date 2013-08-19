#ifndef    page_local_log_sink_Sink_hpp
#   define page_local_log_sink_Sink_hpp

#	include "../stream/Stream.hpp"

namespace page
{
	namespace log
	{
		/**
		 * The base class for sinks.
		 *
		 * @note Although this class doesn't add anything to @c Stream, it is
		 *       provided for consistency with @c Filter.
		 */
		class Sink : public Stream {};
	}
}

#endif
