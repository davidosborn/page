#ifndef    page_local_res_pipe_NullPipe_hpp
#   define page_local_res_pipe_NullPipe_hpp

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct NullPipe : Pipe
		{
			explicit NullPipe(unsigned size = 0);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			unsigned size;
		};
	}
}

#endif
