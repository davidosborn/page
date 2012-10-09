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

#include <algorithm> // find{,_if{,_not}}
#include <fstream> // [io]fstream
#include <unordered_map>
#include "../../err/exception/throw.hpp" // THROW
#include "../../sys/file.hpp" // IsFile, PathExists
#include "../../util/functional.hpp" // isspace_function
#include "../../util/Optional.hpp"
#include "../../util/serialize/deserialize_string.hpp" // Deserialize
#include "../../util/serialize/serialize_string.hpp" // Serialize
#include "../../util/string.hpp" // getline, Partition
#include "ConfSource.hpp"

namespace page
{
	namespace cfg
	{
		namespace
		{
			struct ConfReader : Reader
			{
				ConfReader(const Source &, const std::string &path);

				bool Read(const std::string &name, std::string &);
				bool ReadSeq(const std::string &name, std::vector<std::string> &);

				private:
				typedef std::unordered_map<std::string, std::string> Map;
				Map map;
			};
			struct ConfWriter : Writer
			{
				ConfWriter(const Source &, const std::string &path);
				~ConfWriter();

				void Write(const std::string &name, const std::string &);
				void WriteSeq(const std::string &name, const std::vector<std::string> &);

				void Remove(const std::string &name);

				private:
				std::string path;
				typedef std::unordered_map<std::string, util::Optional<std::string>> Map;
				Map map;
			};

			// reader
			ConfReader::ConfReader(const Source &src, const std::string &path) :
				Reader(src)
			{
				std::ifstream fs(path.c_str());
				if (!fs)
					if (sys::IsFile(path)) THROW err::FileAccessException<>();
					else THROW err::FileNotFoundException<>();
				for (std::string line; util::getline(fs, line);)
					if (!(line = util::Trim(util::Partition(line, '#').first)).empty())
						map.insert(util::Partition(line));
			}
			bool ConfReader::Read(const std::string &name, std::string &str)
			{
				Map::const_iterator iter(map.find(name));
				if (iter != map.end())
				{
					str = iter->second;
					return true;
				}
				return false;
			}
			bool ConfReader::ReadSeq(const std::string &name, std::vector<std::string> &seq)
			{
				Map::const_iterator iter(map.find(name));
				if (iter != map.end())
				{
					seq.clear();
					util::Deserialize(
						iter->second, std::back_inserter(seq),
						util::SequenceDeserializationFlags::none, ',');
					return true;
				}
				return false;
			}

			// writer
			ConfWriter::ConfWriter(const Source &src, const std::string &path) :
				Writer(src), path(path) {}
			ConfWriter::~ConfWriter()
			{
				// load existing file
				typedef std::vector<std::string> Lines;
				Lines lines;
				if (sys::PathExists(path))
				{
					if (!sys::IsFile(path)) THROW err::NotFileException<>();
					std::ifstream fs(path.c_str());
					if (!fs) THROW err::FileAccessException<>();
					for (std::string line; util::getline(fs, line);)
						lines.push_back(line);
				}
				// replace modified cvars
				for (Lines::iterator line(lines.begin()); line != lines.end(); ++line)
				{
					std::string::const_iterator
						keyBegin(std::find_if_not(line->begin(), line->end(), util::isspace_function<char>())),
						keyEnd(std::find_if(keyBegin, static_cast<std::string::const_iterator>(line->end()), util::isspace_function<char>()));
					if (keyBegin != keyEnd)
					{
						Map::const_iterator iter(map.find(std::string(keyBegin, keyEnd)));
						if (iter != map.end())
						{
							if (!iter->second)
							{
								map.erase(iter);
								line = lines.erase(line);
								continue;
							}
							std::string::const_iterator
								valueBegin(std::find_if_not(keyEnd, static_cast<std::string::const_iterator>(line->end()), util::isspace_function<char>())),
								valueEnd(std::find_if_not(
									std::string::const_reverse_iterator(std::find(valueBegin, static_cast<std::string::const_iterator>(line->end()), '#')),
									std::string::const_reverse_iterator(valueBegin),
									util::isspace_function<char>()).base());
							*line =
								std::string(static_cast<std::string::const_iterator>(line->begin()), valueBegin) +
								*iter->second +
								std::string(valueEnd, static_cast<std::string::const_iterator>(line->end()));
							map.erase(iter);
						}
					}
				}
				// append new cvars
				for (Map::const_iterator iter(map.begin()); iter != map.end(); ++iter)
					if (iter->second) lines.push_back(iter->first + ' ' + *iter->second);
				map.clear();
				// update file
				std::ofstream fs(path.c_str());
				if (!fs) THROW err::FileAccessException<>();
				for (Lines::const_iterator line(lines.begin()); line != lines.end(); ++line)
					fs << *line << std::endl;
			}
			void ConfWriter::Write(const std::string &name, const std::string &str)
			{
				map[name] = str;
			}
			void ConfWriter::WriteSeq(const std::string &name, const std::vector<std::string> &seq)
			{
				map[name] = util::Serialize<char>(seq, ',');
			}
			void ConfWriter::Remove(const std::string &name)
			{
				map[name].reset();
			}
		}

		// source
		ConfSource::ConfSource(const std::string &path) : path(path) {}
		Reader *ConfSource::OpenReader() const
		{
			return new ConfReader(*this, path);
		}
		Writer *ConfSource::OpenWriter() const
		{
			return new ConfWriter(*this, path);
		}
		std::string ConfSource::GetName() const
		{
			return path;
		}
	}
}
