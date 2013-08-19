#include <algorithm> // min
#include <cassert>

#include "../../err/Exception.hpp"
#include "../../util/container/ScratchBuffer.hpp"
#include "../adapt/minizip.hpp" // MakeZlibFileFuncDef
#include "Stream.hpp"
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
