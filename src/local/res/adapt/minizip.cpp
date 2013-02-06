/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <minizip/unzip.h>
#include "../../err/Exception.hpp"
#include "../pipe/Pipe.hpp"
#include "../Stream.hpp"

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
