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
#include "../../util/unique_ptr.hpp"
#include "../Stream.hpp"
#include "CatPipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct CatStream : Stream
			{
				CatStream(const Pipe &, const Pipe &);

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				util::unique_ptr<Stream> first, second;
			};

			CatStream::CatStream(const Pipe &first, const Pipe &second) :
				first(first.Open()), second(second.Open()) {}

			void CatStream::DoRead(void *s, unsigned n)
			{
				if (!first->Eof())
				{
					unsigned result = first->ReadSome(s, n);
					s = static_cast<char *>(s) + result;
					n -= result;
				}
				if (n) second->Read(s, n);
			}
			unsigned CatStream::DoReadSome(void *s, unsigned n)
			{
				unsigned result = 0;
				if (!first->Eof())
				{
					result += first->ReadSome(s, n);
					s = static_cast<char *>(s) + result;
					n -= result;
				}
				if (n) result += second->ReadSome(s, n);
				return result;
			}

			unsigned CatStream::DoTell() const
			{
				return first->Tell() + second->Tell();
			}
			unsigned CatStream::DoSize() const
			{
				return first->Size() + second->Size();
			}
			void CatStream::DoSeek(unsigned n)
			{
				unsigned n2 = std::min(first->Size(), n);
				first->Seek(n2);
				second->Seek(n - n2);
			}
		}

		CatPipe::CatPipe(const std::shared_ptr<const Pipe> &first, const std::shared_ptr<const Pipe> &second) :
			first(first), second(second)
		{
			assert(first);
			assert(second);
		}

		unsigned CatPipe::Size() const
		{
			return first->Size() + second->Size();
		}

		Stream *CatPipe::MakeStream() const
			{ return new CatStream(*first, *second); }
	}
}
