#ifndef    page_local_res_pipe_MinizipPipe_hpp
#   define page_local_res_pipe_MinizipPipe_hpp

#	include <memory> // shared_ptr

#	include <minizip/unzip.h> // unz_file_pos

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct MinizipPipe : Pipe
		{
			MinizipPipe(const std::shared_ptr<const Pipe> &, const unz_file_pos &);

			protected:
			Stream *MakeStream() const;

			private:
			std::shared_ptr<const Pipe> super;
			unz_file_pos pos;
		};
	}
}

#endif
