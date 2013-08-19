#include "CurlPipe.hpp"
#include "Stream.hpp"

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
