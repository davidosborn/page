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

#include <algorithm> // min
#include <cassert>
#include "../../err/Exception.hpp"
#include "../../util/unique_ptr.hpp"
#include "../Stream.hpp"
#include "SubPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct SubStream : Stream
			{
				SubStream(const Pipe &, unsigned off, unsigned size);

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				util::unique_ptr<Stream> super;
				unsigned pos, off, size;
			};

			SubStream::SubStream(const Pipe &pipe, unsigned off, unsigned size) :
				super(pipe.Open()), pos(0), off(off), size(size)
			{
				super->Seek(off);
			}

			void SubStream::DoRead(void *s, unsigned n)
			{
				if (pos + n > size)
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
				pos += n;
			}
			unsigned SubStream::DoReadSome(void *s, unsigned n)
			{
				n = super->ReadSome(s, std::min(n, size - pos));
				pos += n;
				return n;
			}

			unsigned SubStream::DoTell() const
			{
				return pos;
			}
			unsigned SubStream::DoSize() const
			{
				return size;
			}
			void SubStream::DoSeek(unsigned n)
			{
				if (n > size)
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
				pos = n;
			}
		}

		SubPipe::SubPipe(const std::shared_ptr<const Pipe> &super, unsigned off) :
			super(super), off(off), size(super->Size() - off)
		{
			assert(super);
		}
		SubPipe::SubPipe(const std::shared_ptr<const Pipe> &super, unsigned off, unsigned size) :
			super(super), off(off), size(size)
		{
			assert(super);
		}

		unsigned SubPipe::Size() const
		{
			return size;
		}

		Stream *SubPipe::MakeStream() const
			{ return new SubStream(*super, off, size); }
	}
}
