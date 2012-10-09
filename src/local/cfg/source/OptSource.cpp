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

#include <cassert>
#include <iterator> // back_inserter
#include "../../opt.hpp" // cfgVars
#include "../../util/serialize/deserialize_string.hpp" // Deserialize
#include "OptSource.hpp"

namespace page
{
	namespace cfg
	{
		namespace
		{
			struct OptReader : Reader
			{
				explicit OptReader(const Source &);

				bool Read(const std::string &name, std::string &);
				bool ReadSeq(const std::string &name, std::vector<std::string> &);
			};

			OptReader::OptReader(const Source &src) : Reader(src) {}
			bool OptReader::Read(const std::string &name, std::string &str)
			{
				for (opt::CfgVars::const_iterator iter(opt::cfgVars.begin()); iter != opt::cfgVars.end(); ++iter)
					if (iter->first == name)
					{
						str = iter->second;
						return true;
					}
				return false;
			}
			bool OptReader::ReadSeq(const std::string &name, std::vector<std::string> &seq)
			{
				for (opt::CfgVars::const_iterator iter(opt::cfgVars.begin()); iter != opt::cfgVars.end(); ++iter)
					if (iter->first == name)
					{
						seq.clear();
						util::Deserialize(
							iter->second, std::back_inserter(seq),
							util::SequenceDeserializationFlags::none, ',');
						return true;
					}
				return false;
			}
		}

		Reader *OptSource::OpenReader() const
		{
			return new OptReader(*this);
		}
		Writer *OptSource::OpenWriter() const
		{
			assert(!"OptSource is read-only");
		}
		std::string OptSource::GetName() const
		{
			return "command line";
		}
	}
}
