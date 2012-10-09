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

#include <fstream>
#include "IniSource.hpp"

namespace page
{
	namespace cfg
	{
		namespace
		{
			struct IniReader : Reader
			{
				IniReader(const Source &, const std::string &path);
				~IniReader();

				bool Read(const std::string &name, std::string &);
				bool ReadSeq(const std::string &, std::vector<std::string> &);

				private:
				std::ifstream fs;
			};
			struct IniWriter : Writer
			{
				IniWriter(const Source &, const std::string &path);
				~IniWriter();

				void Write(const std::string &name, const std::string &);
				void WriteSeq(const std::string &name, const std::vector<std::string> &);

				void Remove(const std::string &name);

				private:
				std::ofstream fs;
			};

			// reader
			IniReader::IniReader(const Source &src, const std::string &path) :
				Reader(src), fs(path.c_str())
			{
				// FIXME: implement
			}
			IniReader::~IniReader()
			{
				// FIXME: implement
			}
			bool IniReader::Read(const std::string &name, std::string &str)
			{
				// FIXME: implement
				return false;
			}
			bool IniReader::ReadSeq(const std::string &name, std::vector<std::string> &seq)
			{
				// FIXME: implement
				return false;
			}

			// writer
			IniWriter::IniWriter(const Source &src, const std::string &path) :
				Writer(src), fs(path.c_str())
			{
				// FIXME: implement
			}
			IniWriter::~IniWriter()
			{
				// FIXME: implement
			}
			void IniWriter::Write(const std::string &name, const std::string &str)
			{
				// FIXME: implement
			}
			void IniWriter::WriteSeq(const std::string &name, const std::vector<std::string> &seq)
			{
				// FIXME: implement
			}
			void IniWriter::Remove(const std::string &name)
			{
				// FIXME: implement
			}
		}

		// source
		IniSource::IniSource(const std::string &path) : path(path) {}
		Reader *IniSource::OpenReader() const
		{
			return new IniReader(*this, path);
		}
		Writer *IniSource::OpenWriter() const
		{
			return new IniWriter(*this, path);
		}
		std::string IniSource::GetName() const
		{
			return path;
		}
	}
}
