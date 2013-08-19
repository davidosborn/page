#include <algorithm> // min
#include <cassert>
#include <cstring> // memcpy

#include "../err/Exception.hpp"
#include "../util/string/operations.hpp" // NormEndl
#include "pipe/Stream.hpp"

namespace page
{
	namespace res
	{
		const unsigned bufferSize = 256;

		// construct/destroy
		Stream::Stream() : bufferPos(0), endl('\0'), eof(false) {}
		Stream::~Stream() {}

		// formatted input
		char Stream::GetChar()
		{
			if (eof) return 0;
			GetChar:
			if (bufferPos == buffer.size())
			{
				if (buffer.empty()) buffer.resize(bufferSize);
				else if (buffer.size() != bufferSize)
				{
					Buffer().swap(buffer);
					bufferPos = 0;
					eof = true;
					return 0;
				}
				buffer.resize(DoReadSome(&*buffer.begin(), buffer.size()));
				if (buffer.empty())
				{
					Buffer().swap(buffer);
					bufferPos = 0;
					eof = true;
					return 0;
				}
				bufferPos = 0;
			}
			char c = buffer[bufferPos++];
			// normalize newline
			switch (c)
			{
				case '\n':
				if (endl == '\r')
				{
					endl = '\0';
					goto GetChar;
				}
				endl = c;
				break;
				case '\r':
				if (endl == '\n')
				{
					endl = '\0';
					goto GetChar;
				}
				endl = c;
				c = '\n';
				break;
			}
			return c;
		}
		std::string Stream::GetLine(char delim)
		{
			std::string s;
			for (char c; (c = GetChar()) != delim && (c || !eof);)
				s.push_back(c);
			return s;
		}
		std::string Stream::GetText()
		{
			/*std::string s;
			for (char c; (c = GetChar()) || !eof;)
				s.push_back(c);
			return s;*/
			// HACK: although the above implementation is more correct, the
			// following should be faster in most situations
			Buffer buffer(Size());
			Read(&*buffer.begin(), buffer.size());
			return util::NormEndl(std::string(buffer.begin(), buffer.end()));
		}

		// unformatted input
		void Stream::Read(void *s, unsigned n)
		{
			if (bufferPos < buffer.size())
			{
				unsigned buffered = std::min(buffer.size() - bufferPos, n);
				std::memcpy(s, &*buffer.begin(), buffered);
				bufferPos += buffered;
				s = static_cast<char *>(s) + buffered;
				if (!(n -= buffered)) return;
			}
			try
			{
				DoRead(s, n);
			}
			catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
			{
				eof = true;
				throw;
			}
		}
		unsigned Stream::ReadSome(void *s, unsigned n)
		{
			unsigned buffered = std::min(buffer.size() - bufferPos, n);
			if (buffered)
			{
				std::memcpy(s, &*buffer.begin(), buffered);
				bufferPos += buffered;
				s = static_cast<char *>(s) + buffered;
				if (!(n -= buffered)) return buffered;
			}
			unsigned result = DoReadSome(s, n);
			if (result < n) eof = true;
			return buffered + result;
		}

		// positioning
		unsigned Stream::Tell() const
		{
			return DoTell() - (buffer.size() - bufferPos);
		}
		unsigned Stream::Size() const
		{
			return DoSize();
		}
		void Stream::Seek(unsigned n)
		{
			// NOTE: there is no guarantee that newlines will be normalized
			// correctly if we seek to the middle of a newline sequence
			endl = false;
			if (bufferPos < buffer.size())
			{
				unsigned pos = Tell();
				if (n < pos ? pos - n <= bufferPos : bufferPos + (n - pos) < buffer.size())
				{
					bufferPos += pos - n;
					return;
				}
				bufferPos = buffer.size();
			}
			try
			{
				DoSeek(n);
			}
			catch (const err::Exception<err::EndOfStreamTag>::Permutation &)
			{
				eof = true;
				throw;
			}
			eof = false;
		}
		void Stream::Seek(int n, SeekOrigin origin)
		{
			switch (origin)
			{
				case begSeekOrigin:
				assert(n >= 0);
				Seek(n);
				break;
				case curSeekOrigin:
				{
					unsigned pos = Tell();
					assert(n >= 0 || static_cast<unsigned>(-n) <= pos);
					Seek(pos + n);
				}
				break;
				case endSeekOrigin:
				{
					unsigned size = Size();
					assert(n >= 0 || static_cast<unsigned>(-n) <= size);
					if (n > 0)
					{
						eof = true;
						THROW((err::Exception<err::ResModuleTag, err::EndOfStreamTag>()))
					}
					Seek(size + n);
				}
				break;
				default: assert(!"invalid seek origin");
			}
		}

		// state
		bool Stream::Eof() const
		{
			return eof;
		}
	}
}
