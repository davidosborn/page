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

#include <vector>
#include "BufferSink.hpp"

namespace page
{
	namespace log
	{
		namespace
		{
			typedef std::vector<char> Buffer;
			Buffer buffer;
		}

		BufferSink::BufferSink(bool err) : err(err) {}

		void BufferSink::Put(char c)
		{
			buffer.push_back(c & 0x7f | err << 7);
		}
		void BufferSink::Put(const char *s, unsigned n)
		{
			for (const char *c = s; c != s + n; c++)
				buffer.push_back(*c & 0x7f | err << 7);
		}

		void BufferSink::Sync()
		{
			buffer.push_back(err << 7);
		}

		void BufferSink::Flush(Stream &out, Stream &err)
		{
			Stream *lastStream = 0;
			for (Buffer::const_iterator c(buffer.begin()); c != buffer.end(); ++c)
			{
				Stream &stream(*c & 0x80 ? err : out);
				if (&stream != lastStream)
				{
					if (lastStream) lastStream->Sync();
					lastStream = &stream;
				}
				char a = *c & 0x7f;
				if (a) stream.Put(a);
				else stream.Sync();
			}
			Buffer().swap(buffer);
		}
	}
}
