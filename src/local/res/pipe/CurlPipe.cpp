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

#include "../Stream.hpp"
#include "CurlPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct CurlStream : Stream
			{
				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				unsigned pos;
			};

			void CurlStream::DoRead(void *s, unsigned n)
			{
				// FIXME: implement
				pos += n;
			}
			unsigned CurlStream::DoReadSome(void *s, unsigned n)
			{
				// FIXME: implement
				pos += n;
				return n;
			}

			unsigned CurlStream::DoTell() const
			{
				return pos;
			}
			unsigned CurlStream::DoSize() const
			{
				// FIXME: implement
				return 0;
			}
			void CurlStream::DoSeek(unsigned n)
			{
				pos += n;
			}
		}

		CurlPipe::CurlPipe(const std::string &address)
		{
			// FIXME: implement
		}

		unsigned CurlPipe::Size() const
		{
			// FIXME: implement
		}

		Stream *CurlPipe::MakeStream() const
			{ return new CurlStream; }
	}
}
