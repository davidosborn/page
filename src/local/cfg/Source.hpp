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

#ifndef    page_cfg_Source_hpp
#   define page_cfg_Source_hpp

#	include <string>
#	include <vector>
#	include "../util/NonCopyable.hpp"

namespace page
{
	namespace cfg
	{
		class Reader;
		class Writer;

		struct Source : util::NonCopyable
		{
			virtual ~Source();

			virtual Reader *OpenReader() const = 0;
			virtual Writer *OpenWriter() const = 0;

			virtual std::string GetName() const = 0;
		};
		struct Reader : util::NonCopyable
		{
			explicit Reader(const Source &);
			virtual ~Reader();

			virtual bool Read(const std::string &name, std::string &) = 0;
			virtual bool ReadSeq(const std::string &name, std::vector<std::string> &) = 0;

			const Source &source;
		};
		struct Writer : util::NonCopyable
		{
			explicit Writer(const Source &);
			virtual ~Writer();

			virtual void Write(const std::string &name, const std::string &) = 0;
			virtual void WriteSeq(const std::string &name, const std::vector<std::string> &) = 0;

			virtual void Remove(const std::string &name) = 0;

			const Source &source;
		};

		// factory functions
		Source *MakeSiteSource();
		Source *MakeUserSource();
		Source *MakeFileSource(const std::string &path);

		// read/write adapters
		template <typename T> bool Read(Reader &, const std::string &name, T &);
		template <typename T> bool Read(Reader &, const std::string &name, std::vector<T> &);
		template <typename T> void Write(Writer &, const std::string &name, const T &);
		template <typename T> void Write(Writer &, const std::string &name, const std::vector<T> &);
	}
}

#	include "Source.tpp"
#endif
