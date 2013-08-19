#ifndef    page_local_res_pipe_SubPipe_hpp
#   define page_local_res_pipe_SubPipe_hpp

#	include <memory> // shared_ptr

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct SubPipe : Pipe
		{
			SubPipe(const std::shared_ptr<const Pipe> &, unsigned off);
			SubPipe(const std::shared_ptr<const Pipe> &, unsigned off, unsigned size);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			std::shared_ptr<const Pipe> super;
			unsigned off, size;
		};
	}
}

#endif
