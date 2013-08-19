#ifndef    page_local_log_stream_BufferStream_hpp
#   define page_local_log_stream_BufferStream_hpp

#	include "BranchableStream.hpp"

namespace page
{
	namespace log
	{
		/**
		 * Stores all data written to it until it is flushed.
		 *
		 * @note Be careful not to insert this stream too early in the chain
		 *       because it will prevent data from reaching time-sensitive
		 *       filters like @c IndentFilter and @c TimeFilter until it is
		 *       flushed.
		 */
		class BufferStream : public BranchableStream
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			using BranchableStream::BranchableStream;

			/*----------------------+
			| Stream implementation |
			+----------------------*/

			private:
			void DoWrite(const std::string &) override;
			void DoFlush() override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::string buffer;
		};
	}
}

#endif
