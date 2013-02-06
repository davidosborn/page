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

// C++
#include <algorithm> // find{,_if_not}
#include <unordered_map>

// Boost
#include <boost/filesystem/fstream.hpp> // [io]fstream

// local
#include "../../err/Exception.hpp"
#include "../../util/string.hpp" // Partition, Trim
#include "../../util/StringBuilder.hpp"
#include "FileSource.hpp"

namespace page
{
	namespace cfg
	{
////////// FileSource::Reader definition ///////////////////////////////////////

		/**
		 * The implementation of @c FileSource's reader.
		 */
		class FileSource::Reader : public Source::Reader
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit Reader(const FileSource &);

			/*----------------------+
			| copy & move semantics |
			+----------------------*/

			public:
			MAKE_UNCOPYABLE(Reader)

			/*------------------------------+
			| Source::Reader implementation |
			+------------------------------*/

			public:
			boost::optional<std::string> Read(const std::string &) override;

			/*-----------------------------------------+
			| Source::ReaderWriter covariant overrides |
			+-----------------------------------------*/

			public:
			const FileSource &GetSource() const;
			
			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * An associative array containing all of the configuration
			 * variables found in the file, represented as key/value pairs.
			 */
			std::unordered_map<std::string, std::string> vars;
		};

////////// FileSource::Writer definition ///////////////////////////////////////
				
		/**
		 * The implementation of @c FileSource's writer.
		 */
		class FileSource::Writer : public Source::Writer
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit Writer(const FileSource &);
			~Writer();

			/*----------------------+
			| copy & move semantics |
			+----------------------*/

			public:
			MAKE_UNCOPYABLE(Writer)

			/*------------------------------+
			| Source::Writer implementation |
			+------------------------------*/

			public:
			void Write(const std::string &key, const std::string &value) override;
			void Remove(const std::string &key) override;

			/*-----------------------------------------+
			| Source::ReaderWriter covariant overrides |
			+-----------------------------------------*/

			public:
			const FileSource &GetSource() const;
			
			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * An associative array containing all of the configuration
			 * variables that have been changed, represented as key/value pairs.
			 */
			std::unordered_map<std::string, boost::optional<std::string>> vars;
		};
	
////////// FileSource //////////////////////////////////////////////////////////

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		FileSource::FileSource(const boost::filesystem::path &path) :
			Source(
				util::StringBuilder<>() << "file:/" << absolute(path).generic_string(),
				util::StringBuilder<>() << "file at '" << absolute(path).string() << "'"),
			path(absolute(path)) {}

		/*----------------------+
		| Source implementation |
		+----------------------*/
		
		std::unique_ptr<Source::Reader> FileSource::OpenReader() const
		{
			return std::unique_ptr<Source::Reader>(new FileSource::Reader(*this));
		}
		
		std::unique_ptr<Source::Writer> FileSource::OpenWriter() const
		{
			return std::unique_ptr<Source::Writer>(new FileSource::Writer(*this));
		}
		
////////// FileSource::Reader implementation ///////////////////////////////////

		FileSource::Reader::Reader(const FileSource &source) :
			Source::Reader(source)
		{
			try
			{
				boost::filesystem::ifstream fs(source.path);
				if (!fs)
					THROW((err::Exception<err::CfgModuleTag, err::FileTag>("failed to open file stream") <<
						boost::errinfo_api_function("std::ifstream::open")))

				for (std::string line; std::getline(fs, line).good();)
				{
					line = util::Partition(line, '#').first; // remove comments
					auto kv = util::Partition(line, '=');    // split into key/value pair
					kv.first  = util::Trim(kv.first);        // trim whitespace around key
					kv.second = util::Trim(kv.second);       // trim whitespace around value
					if (!kv.first.empty())
						vars.insert(kv);
				}
				if (fs.fail() && !fs.eof())
					THROW((err::Exception<err::CfgModuleTag, err::FileReadTag>("error reading file stream")))
			}
			catch (err::Exception<err::FileTag>::Permutation &e)
			{
				e <<
					boost::errinfo_file_name(source.path.string()) <<
					boost::errinfo_file_open_mode("r");
				throw;
			}
		}
		
		boost::optional<std::string> FileSource::Reader::Read(const std::string &key)
		{
			auto iter(vars.find(key));
			if (iter != vars.end())
				return iter->second;
			return boost::none;
		}

////////// FileSource::Writer implementation ///////////////////////////////////

		FileSource::Writer::Writer(const FileSource &source) :
			Source::Writer(source) {}
			
		FileSource::Writer::~Writer()
		{
			// load existing file
			std::vector<std::string> lines;
			try
			{
				boost::filesystem::ifstream fs(GetSource().path);
				if (!fs)
					THROW((err::Exception<err::CfgModuleTag, err::FileTag>("failed to open file stream") <<
						boost::errinfo_api_function("std::ifstream::open")))
				
				for (std::string line; std::getline(fs, line).good();)
					lines.push_back(line);
				if (fs.fail() && !fs.eof())
					THROW((err::Exception<err::CfgModuleTag, err::FileReadTag>("error reading file stream")))
			}
			catch (err::Exception<err::FileTag>::Permutation &e)
			{
				e <<
					boost::errinfo_file_name(GetSource().path.string()) <<
					boost::errinfo_file_open_mode("r");
				throw;
			}

			// replace modified cvars
			for (auto line(lines.begin()); line != lines.end(); ++line)
			{
				auto isspace = [](char c) { return std::isspace(c, std::locale()); };
				auto
					comment  = std::find(line->begin(), line->end(), '#'),
					equal    = std::find(line->begin(), comment,     '='),
					keyBegin = std::find_if_not(line->begin(), equal, isspace),
					keyEnd   = std::find_if_not(
						std::string::reverse_iterator(equal),
						std::string::reverse_iterator(keyBegin),
						isspace).base();
				if (keyBegin != keyEnd)
				{
					std::string key(keyBegin, keyEnd);
					auto iter(vars.find(key));
					if (iter != vars.end())
					{
						if (!iter->second)
						{
							vars.erase(iter);
							line = lines.erase(line);
							continue;
						}
						auto
							valueBegin = std::find_if_not(std::next(equal), comment, isspace),
							valueEnd   = std::find_if_not(
								std::string::reverse_iterator(comment),
								std::string::reverse_iterator(valueBegin),
								isspace).base();
						*line =
							std::string(line->begin(), valueBegin) +
							*iter->second                          +
							std::string(valueEnd, line->end());
						vars.erase(iter);
					}
				}
			}

			// append new cvars
			for (const auto &kv : vars)
				if (kv.second)
					lines.push_back(kv.first + " = " + *kv.second);
			vars.clear();

			// update file
			boost::filesystem::ofstream fs(GetSource().path);
			if (!fs)
				THROW((err::Exception<err::CfgModuleTag, err::FileTag>("failed to open file stream") <<
					boost::errinfo_api_function("std::ifstream::open") <<
					boost::errinfo_file_name(GetSource().path.string()) <<
					boost::errinfo_file_open_mode("w")))
			for (const auto &line : lines)
				fs << line << std::endl;
		}

		void FileSource::Writer::Write(const std::string &key, const std::string &value)
		{
			vars[key] = value;
		}

		void FileSource::Writer::Remove(const std::string &key)
		{
			vars[key] = boost::none;
		}
	}
}
