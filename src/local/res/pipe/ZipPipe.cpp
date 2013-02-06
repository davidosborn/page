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
#include <zip.h>
#include "../../err/Exception.hpp"
#include "../../sys/file.hpp" // FileSize
#include "../../util/buffer.hpp" // GetDirtyBuffer
#include "../adapt/zip.hpp" // ZipError
#include "../Stream.hpp"
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
					unsigned n2 = std::min(n, util::GetDirtyBuffer().size());
					int result = zip_fread(file, &*util::GetDirtyBuffer().begin(), n2);
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
