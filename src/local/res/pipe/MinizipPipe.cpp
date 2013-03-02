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
 *
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

#include <algorithm> // min
#include <cassert>

#include "../../err/Exception.hpp"
#include "../../util/container/ScratchBuffer.hpp"
#include "../adapt/minizip.hpp" // MakeZlibFileFuncDef
#include "../Stream.hpp"
#include "MinizipPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct MinizipStream : Stream
			{
				MinizipStream(const Pipe &, const unz_file_pos &);
				~MinizipStream();

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				void Reset();

				unzFile file;
				unsigned pos, size;
			};

			MinizipStream::MinizipStream(const Pipe &pipe, const unz_file_pos &pos) : pos(0)
			{
				zlib_filefunc_def zlibFileFuncDef(MakeZlibFileFuncDef(pipe));
				// FIXME: if pipe knows its path, pass it here
				file = unzOpen2("", &zlibFileFuncDef);
				unzGoToFilePos(file, const_cast<unz_file_pos *>(&pos));
				unz_file_info info;
				unzGetCurrentFileInfo(file, &info, 0, 0, 0, 0, 0, 0);
				size = info.uncompressed_size;
				unzOpenCurrentFile(file);
			}
			MinizipStream::~MinizipStream()
			{
				unzCloseCurrentFile(file);
				unzClose(file);
			}

			void MinizipStream::DoRead(void *s, unsigned n)
			{
				int result = unzReadCurrentFile(file, s, n);
				if (result < 0)
					THROW((err::Exception<err::ResModuleTag, err::MinizipPlatformTag, err::StreamReadTag>()))
				pos += result;
				if (result < n)
					THROW((err::Exception<err::ResModuleTag, err::MinizipPlatformTag, err::EndOfStreamTag>()))
			}
			unsigned MinizipStream::DoReadSome(void *s, unsigned n)
			{
				int result = unzReadCurrentFile(file, s, n);
				if (result < 0)
					THROW((err::Exception<err::ResModuleTag, err::MinizipPlatformTag, err::StreamReadTag>()))
				pos += result;
				return result;
			}

			unsigned MinizipStream::DoTell() const
			{
				return pos;
			}
			unsigned MinizipStream::DoSize() const
			{
				return size;
			}
			void MinizipStream::DoSeek(unsigned n)
			{
				if (n < pos) Reset();
				else n -= pos;
				while (n)
				{
					unsigned n2 = std::min(n, GLOBAL(util::ScratchBuffer).size());
					int result = unzReadCurrentFile(file, GLOBAL(util::ScratchBuffer).data(), n2);
					if (result < 0)
						THROW((err::Exception<err::ResModuleTag, err::MinizipPlatformTag, err::StreamReadTag>()))
					pos += result;
					if (result < n2)
						THROW((err::Exception<err::ResModuleTag, err::MinizipPlatformTag, err::EndOfStreamTag>()))
					n -= n2;
				}
			}

			void MinizipStream::Reset()
			{
				unzCloseCurrentFile(file);
				unzOpenCurrentFile(file);
				pos = 0;
			}
		}

		MinizipPipe::MinizipPipe(const std::shared_ptr<const Pipe> &super, const unz_file_pos &pos) :
			super(super), pos(pos)
		{
			assert(super);
		}

		Stream *MinizipPipe::MakeStream() const
			{ return new MinizipStream(*super, pos); }
	}
}
