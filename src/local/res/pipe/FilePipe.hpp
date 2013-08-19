#ifndef    page_local_res_pipe_FilePipe_hpp
#   define page_local_res_pipe_FilePipe_hpp

#	include <string>

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct FilePipe : Pipe
		{
			explicit FilePipe(const std::string &);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			std::string path;
		};
	}
}

#endif
