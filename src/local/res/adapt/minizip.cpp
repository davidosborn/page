#include <minizip/unzip.h>

#include "../../err/Exception.hpp"
#include "../pipe/Pipe.hpp"
#include "../pipe/Stream.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			Stream *MinizipOpen(const Pipe *pipe, const char *s, int mode)
			{
				return (mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) == ZLIB_FILEFUNC_MODE_READ ? pipe->Open() : 0;
			}
			uLong MinizipRead(const Pipe *, Stream *stream, void *s, uLong n)
			{
				return stream->ReadSome(s, n);
			}
			long MinizipTell(const Pipe *, Stream *stream)
			{
				return stream->Tell();
			}
			long MinizipSeek(const Pipe *, Stream *stream, uLong off, int origin)
			{
				Stream::SeekOrigin streamOrigin;
				switch (origin)
				{
					case ZLIB_FILEFUNC_SEEK_SET: streamOrigin = Stream::begSeekOrigin; break;
					case ZLIB_FILEFUNC_SEEK_CUR: streamOrigin = Stream::curSeekOrigin; break;
					case ZLIB_FILEFUNC_SEEK_END: streamOrigin = Stream::endSeekOrigin; break;
					default: return -1;
				}
				stream->Seek(off, streamOrigin);
				return 0;
			}
			int MinizipClose(const Pipe *, Stream *stream)
			{
				delete stream;
			}
			int MinizipError(const Pipe *, Stream *stream)
			{
				THROW((err::Exception<err::ResModuleTag, err::MinizipTag>()))
			}
		}

		zlib_filefunc_def MakeZlibFileFuncDef(const Pipe &pipe)
		{
			zlib_filefunc_def zlibFileFuncDef =
			{
				reinterpret_cast<open_file_func>(MinizipOpen),
				reinterpret_cast<read_file_func>(MinizipRead),
				0,
				reinterpret_cast<tell_file_func>(MinizipTell),
				reinterpret_cast<seek_file_func>(MinizipSeek),
				reinterpret_cast<close_file_func>(MinizipClose),
				reinterpret_cast<testerror_file_func>(MinizipError),
				const_cast<Pipe *>(&pipe)
			};
			return zlibFileFuncDef;
		}
	}
}
