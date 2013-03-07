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
#include <cstring> // memcpy
#include <memory> // unique_ptr

#include "../../err/Exception.hpp"
#include "Stream.hpp"
#include "Pipe.hpp"

namespace page
{
	namespace res
	{
		namespace detail
		{
			/**
			 * Provides stream access to the locked portion of a pipe, opening
			 * a real stream when data is requested beyond that point.
			 */
			struct LockStream : Stream
			{
				// constructor
				explicit LockStream(const Pipe &, unsigned size);

				// input
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				// positioning
				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				const Pipe &pipe;
				std::unique_ptr<Stream> stream;
				unsigned pos, size;
			};

			// constructor
			LockStream::LockStream(const Pipe &pipe, unsigned size) :
				pipe(pipe), pos(0), size(size) {}

			// input
			void LockStream::DoRead(void *s, unsigned n)
			{
				if (pos < pipe.lockBuffer.size())
				{
					unsigned buffered = std::min(pipe.lockBuffer.size() - pos, n);
					std::memcpy(s, &pipe.lockBuffer[pos], buffered);
					if (stream) stream->Seek(buffered, curSeekOrigin);
					pos += buffered;
					s = static_cast<char *>(s) + buffered;
					if (!(n -= buffered)) return;
				}
				if (!stream)
				{
					stream.reset(pipe.MakeStream());
					stream->Seek(pos);
				}
				try
				{
					stream->Read(s, n);
				}
				catch (const Exception<err::EndOfStreamTag>::Permutation &)
				{
					pos = stream->Tell();
					throw;
				}
				pos += n;
			}
			unsigned LockStream::DoReadSome(void *s, unsigned n)
			{
				unsigned buffered = 0;
				if (pos < pipe.lockBuffer.size())
				{
					buffered = std::min(pipe.lockBuffer.size() - pos, n);
					std::memcpy(s, &pipe.lockBuffer[pos], buffered);
					if (stream) stream->Seek(buffered, curSeekOrigin);
					pos += buffered;
					s = static_cast<char *>(s) + buffered;
					if (!(n -= buffered) || pos == size) return buffered;
				}
				if (!stream)
				{
					stream.reset(pipe.MakeStream());
					stream->Seek(pos);
				}
				unsigned result = stream->ReadSome(s, n);
				pos += result;
				return buffered + result;
			}

			// positioning
			unsigned LockStream::DoTell() const
			{
				return stream.get() ? stream->Tell() : pos;
			}
			unsigned LockStream::DoSize() const
			{
				return stream.get() ? stream->Size() : size;
			}
			void LockStream::DoSeek(unsigned n)
			{
				if (stream)
				{
					try
					{
						stream->Seek(n);
					}
					catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
					{
						pos = stream->Tell();
						throw;
					}
				}
				pos = n;
			}
		}

		// stream access
		Stream *Pipe::Open() const
		{
			return !lockBuffer.empty() ? new detail::LockStream(*this, lockBuffer.size()) : MakeStream();
		}
		unsigned Pipe::Size() const
		{
			return const std::unique_ptr<Stream>(Open())->Size();
		}

		// preloading
		void Pipe::Lock(unsigned size)
		{
			LockBuffer::size_type prevLockSize(lockBuffer.size());
			if (size > prevLockSize)
			{
				lockBuffer.resize(size);
				const std::unique_ptr<Stream> stream(MakeStream());
				if (prevLockSize) stream->Seek(prevLockSize);
				lockBuffer.resize(prevLockSize +
					stream->ReadSome(&lockBuffer[prevLockSize], size - prevLockSize));
			}
		}
		void Pipe::Unlock()
		{
			LockBuffer().swap(lockBuffer);
		}

		// pipe locker
		// constructors
		PipeLocker::PipeLocker() {}
		PipeLocker::PipeLocker(Pipe &pipe, unsigned size)
		{
			Reset(pipe, size);
		}

		// modifiers
		void PipeLocker::Reset()
		{
			Base::Reset();
		}
		void PipeLocker::Reset(Pipe &pipe, unsigned size)
		{
			this->pipe = &pipe;
			this->size = size;
			Base::Reset(true);
		}
		void PipeLocker::Save()
		{
			pipe->Lock(size);
		}
		void PipeLocker::Load()
		{
			pipe->Unlock();
		}
	}
}
