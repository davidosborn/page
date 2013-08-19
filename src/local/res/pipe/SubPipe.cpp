#include <algorithm> // min
#include <cassert>
#include <memory> // unique_ptr

#include "../../err/Exception.hpp"
#include "Stream.hpp"
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
				std::unique_ptr<Stream> super;
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
