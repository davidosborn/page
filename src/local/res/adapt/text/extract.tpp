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

#include <algorithm> // fill
#include <iterator> // iterator_traits
#include "../../../err/Exception.hpp"
#include "../../../util/lexical_cast.hpp"
#include "../../../util/string.hpp" // Partition

namespace page
{
	namespace res
	{
		template <typename OutputIterator> void Extract(const std::string &value, OutputIterator iter, OutputIterator end)
		{
			if (iter == end)
			{
				if (!value.empty())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("too many arguments")))
				return;
			}
			typedef typename std::iterator_traits<OutputIterator>::value_type Type;
			util::Partitioned part(util::Partition(value));
			if (!part.second.empty())
			{
				for (;; part = util::Partition(part.second))
				{
					*iter = util::lexical_cast<Type>(part.first);
					if (++iter == end) break;
					if (part.second.empty())
						THROW((err::Exception<err::ResModuleTag, err::FormatTag>("not enough arguments")))
				}
				if (!part.second.empty())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("too many arguments")))
			}
			else std::fill(iter, end, util::lexical_cast<Type>(part.first));
		}
		template <typename Container> void Extract(const std::string &value, Container &container)
		{
			return Extract(value, container.begin(), container.end());
		}
	}
}
