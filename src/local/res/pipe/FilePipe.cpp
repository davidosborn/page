#include <fstream>

#include "../../err/Exception.hpp"
#include "../../sys/file.hpp" // FileSize, IsFile
#include "Stream.hpp"
#include "FilePipe.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			struct FileStream : Stream
			{
				explicit FileStream(const std::string &path);

				protected:
				void DoRead(void *, unsigned);
				unsigned DoReadSome(void *, unsigned);

				unsigned DoTell() const;
				unsigned DoSize() const;
				void DoSeek(unsigned);

				private:
				// HACK: mutable to avoid const-correctness issues (see N1360)
				mutable std::ifstream fs;
				unsigned size;
			};

			FileStream::FileStream(const std::string &path) :
				fs(path, std::ios_base::binary)
			{
				if (!fs)
					if (sys::IsFile(path))
						THROW((err::Exception<err::ResModuleTag, err::FileAccessTag>()))
					else
						THROW((err::Exception<err::ResModuleTag, err::FileNotFoundTag>()))
				// determine file size
				fs.seekg(0, std::ifstream::end);
				size = fs.tellg();
				fs.seekg(0);
			}

			void FileStream::DoRead(void *s, unsigned n)
			{
				if (!fs) fs.clear();
				if (!fs.read(static_cast<char *>(s), n))
					if (fs.eof())
						THROW((err::Exception<err::ResModuleTag, err::EndOfFileTag>()))
					else
						THROW((err::Exception<err::ResModuleTag, err::FileReadTag>()))
			}
			unsigned FileStream::DoReadSome(void *s, unsigned n)
			{
				if (!fs) fs.clear();
				if (fs.read(static_cast<char *>(s), n).bad())
					THROW((err::Exception<err::ResModuleTag, err::FileReadTag>()))
				return fs.gcount();
			}

			unsigned FileStream::DoTell() const
			{
				if (!fs || fs.eof()) fs.clear();
				std::ifstream::pos_type pos = fs.tellg();
				if (pos == std::ifstream::pos_type(-1))
					THROW((err::Exception<err::ResModuleTag, err::FileTellTag>()))
				return pos;
			}
			unsigned FileStream::DoSize() const
			{
				return size;
			}
			void FileStream::DoSeek(unsigned n)
			{
				if (!fs || fs.eof()) fs.clear();
				if (!fs.seekg(n))
					if (fs.eof())
						THROW((err::Exception<err::ResModuleTag, err::EndOfFileTag, err::Tag>()))
					else
						THROW((err::Exception<err::ResModuleTag, err::FileSeekTag, err::Tag>()))
			}
		}

		FilePipe::FilePipe(const std::string &path) : path(path) {}

		unsigned FilePipe::Size() const
		{
			return sys::FileSize(path);
		}

		Stream *FilePipe::MakeStream() const
			{ return new FileStream(path); }
	}
}
