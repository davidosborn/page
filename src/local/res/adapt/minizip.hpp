#ifndef    page_local_res_adapt_minizip_hpp
#   define page_local_res_adapt_minizip_hpp

#	include <minizip/unzip.h> // zlib_filefunc_def

namespace page
{
	namespace res
	{
		class Pipe;

		zlib_filefunc_def MakeZlibFileFuncDef(const Pipe &);
	}
}

#endif
