#include <algorithm> // min

#include <zip.h>

#include "../../err/Exception.hpp"
#include "../../sys/file.hpp" // FileSize
#include "../../util/container/ScratchBuffer.hpp"
#include "../adapt/zip.hpp" // ZipError
#include "Stream.hpp"
#include "ZipPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct ZipStream : Stream
			{
				ZipStream(const std::string &path, int index);
				~ZipStream();

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				void Reset();

				zip *archive;
				zip_file *file;
				int index; // for resetting
				unsigned pos, size;
			};

			ZipStream::ZipStream(const std::string &path, int index) :
				index(index), pos(0)
			{
				int ze = 0;
				if (!(archive = zip_open(path.c_str(), 0, &ze))) ZipError(ze);
				struct zip_stat stat;
				if (zip_stat_index(archive, index, 0, &stat) == -1) ZipError(archive);
				size = stat.size;
				if (!(file = zip_fopen_index(archive, index, 0))) ZipError(archive);
			}
			ZipStream::~ZipStream()
			{
				int ze = zip_fclose(file);
				if (ze) ZipError(ze);
				if (zip_close(archive) == -1) ZipError(archive);
			}

			void ZipStream::DoRead(void *s, unsigned n)
			{
				int result = zip_fread(file, s, n);
				if (result == -1)
					THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::StreamReadTag>()))
				pos += result;
				if (result < n)
					THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::EndOfStreamTag>()))
			}
			unsigned ZipStream::DoReadSome(void *s, unsigned n)
			{
				int result = zip_fread(file, s, n);
				if (result == -1)
					THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::StreamReadTag>()))
				pos += result;
				return result;
			}

			unsigned ZipStream::DoTell() const
			{
				return pos;
			}
			unsigned ZipStream::DoSize() const
			{
				return size;
			}
			void ZipStream::DoSeek(unsigned n)
			{
				if (n < pos) Reset();
				else n -= pos;
				while (n)
				{
					unsigned n2 = std::min(n, GLOBAL(util::ScratchBuffer).size());
					int result = zip_fread(file, GLOBAL(util::ScratchBuffer).data(), n2);
					if (result == -1)
						THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::StreamReadTag>()))
					pos += result;
					if (result < n2)
						THROW((err::Exception<err::ResModuleTag, err::ZipPlatformTag, err::EndOfStreamTag>()))
					n -= n2;
				}
			}

			void ZipStream::Reset()
			{
				int ze = zip_fclose(file);
				if (ze) ZipError(ze);
				if (!(file = zip_fopen_index(archive, index, 0))) ZipError(archive);
				pos = 0;
			}
		}

		ZipPipe::ZipPipe(const std::string &path, int index) :
			path(path), index(index) {}

		Stream *ZipPipe::MakeStream() const
			{ return new ZipStream(path, index); }
	}
}
