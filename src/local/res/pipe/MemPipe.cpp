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
#include <cstring> // memcpy
#include "../../err/exception/throw.hpp" // THROW
#include "../Stream.hpp"
#include "MemPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct MemStream : Stream
			{
				MemStream(const std::string &);

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				const std::string &data;
				unsigned pos;
			};

			MemStream::MemStream(const std::string &data) :
				data(data), pos(0) {}

			void MemStream::DoRead(void *s, unsigned n)
			{
				unsigned n2 = std::min(data.size() - pos, n);
				std::memcpy(s, data.data() + pos, n2);
				pos += n2;
				if (n > n2) THROW err::EndOfStreamException<err::ResourceTag>();
			}
			unsigned MemStream::DoReadSome(void *s, unsigned n)
			{
				if (pos + n > data.size()) n = data.size() - pos;
				std::memcpy(s, data.data() + pos, n);
				pos += n;
				return n;
			}

			unsigned MemStream::DoTell() const
			{
				return pos;
			}
			unsigned MemStream::DoSize() const
			{
				return data.size();
			}
			void MemStream::DoSeek(unsigned n)
			{
				if (n > data.size())
				{
					pos = data.size();
					THROW err::EndOfStreamException<err::ResourceTag>();
				}
				pos = n;
			}
		}

		MemPipe::MemPipe(const std::string &data) : data(data) {}
		MemPipe::MemPipe(const void *s, unsigned n) :
			data(static_cast<const char *>(s), n) {}

		unsigned MemPipe::Size() const
		{
			return data.size();
		}

		Stream *MemPipe::MakeStream() const
			{ return new MemStream(data); }
	}
}
