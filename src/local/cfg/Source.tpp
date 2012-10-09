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

#include <algorithm> // transform
#include <iterator> // back_inserter
#include "../util/lexical_cast.hpp"

namespace page
{
	namespace cfg
	{
		// read/write adapters
		template <typename T> bool Read(Reader &reader, const std::string &name, T &val)
		{
			std::string str;
			if (reader.Read(name, str))
			{
				val = str.empty() ? T() : util::lexical_cast<T>(str);
				return true;
			}
			return false;
		}
		template <typename T> bool Read(Reader &reader, const std::string &name, std::vector<T> &val)
		{
			std::vector<std::string> seq;
			if (reader.ReadSeq(name, seq))
			{
				val.clear();
				std::transform(seq.begin(), seq.end(), std::back_inserter(val),
					util::lexical_cast_function<T, std::string>());
				return true;
			}
			return false;
		}
		template <typename T> void Write(Writer &writer, const std::string &name, const T &val)
		{
			writer.Write(name, util::lexical_cast<std::string>(val));
		}
		template <typename T> void Write(Writer &writer, const std::string &name, const std::vector<T> &val)
		{
			std::vector<std::string> seq;
			std::transform(val.begin(), val.end(), std::back_inserter(seq),
				util::lexical_cast_function<std::string, T>());
			writer.WriteSeq(name, seq);
		}
	}
}
