#ifndef    page_local_res_pipe_CatPipe_hpp
#   define page_local_res_pipe_CatPipe_hpp

#	include <memory> // shared_ptr

#	include "Pipe.hpp"

namespace page
{
	namespace res
	{
		struct CatPipe : Pipe
		{
			CatPipe(const std::shared_ptr<const Pipe> &, const std::shared_ptr<const Pipe> &);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			std::shared_ptr<const Pipe> first, second;
		};
	}
}

#endif
