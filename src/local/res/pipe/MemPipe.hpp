#ifndef    page_local_res_pipe_MemPipe_hpp
#   define page_local_res_pipe_MemPipe_hpp

#	include <string>

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct MemPipe : Pipe
		{
			explicit MemPipe(const std::string &);
			MemPipe(const void *, unsigned);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			std::string data;
		};
	}
}

#endif
