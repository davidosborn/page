#include "BufferStream.hpp"

namespace page
{
	namespace log
	{
		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void BufferStream::DoWrite(const std::string &s)
		{
			buffer.append(s);
		}

		void BufferStream::DoFlush()
		{
			BranchableStream::DoWrite(buffer);
			buffer.clear();
			BranchableStream::DoFlush();
		}
	}
}
