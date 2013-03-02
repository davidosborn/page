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
 *
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

#include "../../err/Exception.hpp"
#include "../../util/io/deserialize.hpp"
#include "Source.hpp"

namespace page
{
	namespace cfg
	{
////////// Source //////////////////////////////////////////////////////////////

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Source::Source(
			const std::string &uri,
			const std::string &friendlyName) :
				uri(uri),
				friendlyName(friendlyName) {}

		/*----------+
		| observers |
		+----------*/

		const std::string &Source::GetUri() const
		{
			return uri;
		}

		const std::string &Source::GetFriendlyName() const
		{
			return friendlyName;
		}

		/*----+
		| I/O |
		+----*/

		std::unique_ptr<Source::Writer> Source::OpenWriter() const
		{
			THROW((err::Exception<err::CfgModuleTag, err::ReadOnlyTag>("source is read-only")))
		}

////////// Source::ReaderWriter ////////////////////////////////////////////////

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Source::ReaderWriter::ReaderWriter(const Source &source) :
			source(source) {}

		/*----------+
		| observers |
		+----------*/

		const Source &Source::ReaderWriter::GetSource() const
		{
			return source;
		}

////////// Source::Reader //////////////////////////////////////////////////////

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Source::Reader::Reader(const Source &source) :
			ReaderWriter(source) {}

		/*-----------+
		| operations |
		+-----------*/

		boost::optional<std::vector<std::string>> Source::Reader::ReadSequence(const std::string &key)
		{
			auto value(Read(key));
			if (!value)
				return boost::none;

			std::vector<std::string> sequence;
			util::Deserialize(*value, sequence, util::SequenceDeserializationFlags::none, ',');
			return sequence;
		}

////////// Source::Writer //////////////////////////////////////////////////////

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Source::Writer::Writer(const Source &source) :
			ReaderWriter(source) {}

		/*-----------+
		| operations |
		+-----------*/

		void Source::Writer::WriteSequence(const std::string &key, const std::string &value)
		{
			// FIXME: implement
			assert(false);
		}
	}
}
