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
#include <cstring> // memset
#include "../../err/exception/throw.hpp" // THROW
#include "../Stream.hpp"
#include "NullPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct NullStream : Stream
			{
				explicit NullStream(unsigned size = 0);

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				unsigned pos, size;
			};

			NullStream::NullStream(unsigned size) : pos(0), size(size) {}

			void NullStream::DoRead(void *s, unsigned n)
			{
				unsigned n2 = size ? std::min(size - pos, n) : n;
				std::memset(s, 0, n2);
				pos += n2;
				if (n > n2) THROW err::EndOfStreamException<err::ResourceTag>();
			}
			unsigned NullStream::DoReadSome(void *s, unsigned n)
			{
				if (size && pos + n > size) n = size - pos;
				std::memset(s, 0, n);
				pos += n;
				return n;
			}

			unsigned NullStream::DoTell() const
			{
				return pos;
			}
			unsigned NullStream::DoSize() const
			{
				return size;
			}
			void NullStream::DoSeek(unsigned n)
			{
				if (size && n > size)
				{
					pos = size;
					THROW err::EndOfStreamException<err::ResourceTag>();
				}
				pos = n;
			}
		}

		NullPipe::NullPipe(unsigned size) : size(size) {}

		unsigned NullPipe::Size() const
		{
			return size;
		}

		Stream *NullPipe::MakeStream() const
			{ return new NullStream(size); }
	}
}
