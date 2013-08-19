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
