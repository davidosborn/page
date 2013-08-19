#ifndef    page_local_res_pipe_CurlPipe_hpp
#   define page_local_res_pipe_CurlPipe_hpp

#	include <string>

#	include <curl/curl.h>

#	include "../pipe/Pipe.hpp"

namespace page
{
	namespace res
	{
		struct CurlPipe : Pipe
		{
			explicit CurlPipe(const std::string &);

			unsigned Size() const;

			protected:
			Stream *MakeStream() const;

			private:
			// FIXME: curl data goes here
		};
	}
}

#endif
