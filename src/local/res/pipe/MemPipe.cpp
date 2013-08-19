#include <algorithm> // min
#include <cstring> // memcpy

#include "../../err/Exception.hpp"
#include "Stream.hpp"
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
				if (n > n2)
					THROW((err::Exception<err::ResModuleTag, err::EndOfStreamTag>()))
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
					THROW((err::Exception<err::ResModuleTag, err::EndOfStreamTag>()))
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
