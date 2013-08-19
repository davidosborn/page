#ifndef    page_local_res_pipe_ZipPipe_hpp
#   define page_local_res_pipe_ZipPipe_hpp

#	include <string>

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct ZipPipe : Pipe
		{
			ZipPipe(const std::string &, int index);

			protected:
			Stream *MakeStream() const;

			private:
			std::string path;
			int index;
		};
	}
}

#endif
