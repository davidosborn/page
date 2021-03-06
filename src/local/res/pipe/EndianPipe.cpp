/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>
#include <memory> // unique_ptr

#include "EndianPipe.hpp"
#include "Stream.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct EndianStream : Stream
			{
				EndianStream(const Pipe &, const std::string &format,
					util::Endian source, util::Endian destination);

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				std::unique_ptr<Stream> super;
				unsigned pos;
				std::string format;
				util::Endian srcEndian, destEndian;
			};

			EndianStream::EndianStream(const Pipe &pipe, const std::string &format, util::Endian srcEndian, util::Endian destEndian) :
				super(pipe.Open()), pos(0), format(format),
				srcEndian(srcEndian), destEndian(destEndian) {}

			void EndianStream::DoRead(void *s, unsigned n)
			{
				super->Read(s, n);
				// FIXME: swap bytes as specified in format
				pos += n;
				// FIXME: reference code ripped from SubPipe
				/*if (pos + n > size)
				{
					try
					{
						super->Read(s, size - pos);
					}
					catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
					{
						pos = super->Tell() - off;
						throw;
					}
					pos = size;
					THROW((err::Exception<err::ResModuleTag, err::EndOfStreamTag>()))
				}
				try
				{
					super->Read(s, n);
				}
				catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
				{
					pos = super->Tell() - off;
					throw;
				}
				pos += n;*/
			}
			unsigned EndianStream::DoReadSome(void *s, unsigned n)
			{
				n = super->ReadSome(s, n);
				// FIXME: swap bytes as specified in format
				pos += n;
				return n;
			}

			unsigned EndianStream::DoTell() const
			{
				// FIXME: this assumes we don't modify the stream position to
				// swap bytes outside the current range
				return super->Tell();
			}
			unsigned EndianStream::DoSize() const
			{
				return super->Size();
			}
			void EndianStream::DoSeek(unsigned n)
			{
				// FIXME: this assumes we don't modify the stream position to
				// swap bytes outside the current range
				super->Seek(n);
				pos = n;
				// FIXME: reference code ripped from SubPipe
				/*if (n > size)
				{
					try
					{
						super->Seek(off + size);
					}
					catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
					{
						pos = super->Tell() - off;
						throw;
					}
					pos = size;
					THROW((err::Exception<err::ResModuleTag, err::EndOfStreamTag>()))
				}
				try
				{
					super->Seek(off + n);
				}
				catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
				{
					pos = super->Tell() - off;
					throw;
				}
				pos = n;*/
			}
		}

		EndianPipe::EndianPipe(const std::shared_ptr<const Pipe> &super, const std::string &format, util::Endian srcEndian, util::Endian destEndian) :
			super(super), format(format), srcEndian(srcEndian), destEndian(destEndian)
		{
			assert(super);
		}

		unsigned EndianPipe::Size() const
		{
			return super->Size();
		}

		Stream *EndianPipe::MakeStream() const
			{ return new EndianStream(*super, format, srcEndian, destEndian); }
	}
}
