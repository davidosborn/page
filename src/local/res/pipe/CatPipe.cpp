#include <algorithm> // min
#include <cassert>
#include <memory> // unique_ptr

#include "CatPipe.hpp"
#include "Stream.hpp"

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
				std::unique_ptr<Stream> first, second;
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
